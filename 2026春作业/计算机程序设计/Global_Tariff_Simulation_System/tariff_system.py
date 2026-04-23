#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
全球关税模拟系统 - 支持命令行与 tkinter 图形界面
实现国家管理、商品管理、关税管理、路径价格计算、最优采购路径查询、
制造业回流分析及数据持久化。
"""

import json
import os
import sys
import tkinter as tk
from tkinter import ttk, messagebox, simpledialog, scrolledtext
from typing import Dict, List, Tuple, Optional, Set, Any
from dataclasses import dataclass, asdict
from itertools import permutations
import heapq


@dataclass
class Country:
    """国家实体"""
    name: str           # 中文名称
    code: str           # 英文代码（小写存储）
    transit_fee: float  # 过境费（单件商品）


@dataclass
class Commodity:
    """商品类型"""
    name: str           # 商品名称（小写存储）


class TariffSystem:
    """全球关税模拟系统核心类（与GUI共用）"""

    def __init__(self, data_file: str = "tariff_data.json"):
        self.data_file = data_file
        self.countries: Dict[str, Country] = {}       # code -> Country
        self.commodities: Dict[str, Commodity] = {}   # name_lower -> Commodity
        # 生产价格: (country_code, commodity_name_lower) -> price
        self.production_prices: Dict[Tuple[str, str], float] = {}
        # 关税: (export_code, import_code, commodity_name_lower) -> rate (%)
        self.tariffs: Dict[Tuple[str, str, str], float] = {}
        self.history: list = []
        # 用于记录操作历史，支持撤销功能
        self._batch_mode = False      # 是否处于批量模式
        self._batch_changes = []      # 批量模式下暂存的变更
        self.load_data()

    # ------------------- 数据持久化 -------------------
    def save_data(self):
        """保存所有数据到JSON文件"""
        data = {
            "countries": {code: asdict(c) for code, c in self.countries.items()},
            "commodities": {name: asdict(c) for name, c in self.commodities.items()},
            "production_prices": {f"{c}||{m}": p for (c, m), p in self.production_prices.items()},
            "tariffs": {f"{e}||{i}||{m}": r for (e, i, m), r in self.tariffs.items()}
        }
        with open(self.data_file, 'w', encoding='utf-8') as f:
            json.dump(data, f, indent=2, ensure_ascii=False)

    def load_data(self):
        """从JSON文件加载数据，若文件不存在则初始化空系统"""
        if not os.path.exists(self.data_file):
            return
        try:
            with open(self.data_file, 'r', encoding='utf-8') as f:
                data = json.load(f)
            # 加载国家
            self.countries.clear()
            for code, cdata in data.get("countries", {}).items():
                self.countries[code] = Country(**cdata)
            # 加载商品
            self.commodities.clear()
            for name, cdata in data.get("commodities", {}).items():
                self.commodities[name] = Commodity(**cdata)
            # 加载生产价格
            self.production_prices.clear()
            for key, price in data.get("production_prices", {}).items():
                c, m = key.split('||', 1)
                self.production_prices[(c, m)] = price
            # 加载关税
            self.tariffs.clear()
            for key, rate in data.get("tariffs", {}).items():
                e, i, m = key.split('||', 2)
                self.tariffs[(e, i, m)] = rate
        except Exception as e:
            print(f"加载数据失败: {e}，将使用空系统")

    # ------------------- 国家管理 -------------------
    def add_country(self, name: str, code: str, transit_fee: float) -> Tuple[bool, str]:
        """返回 (成功标志, 消息)"""
        code_lower = code.lower()
        if code_lower in self.countries:
            return False, f"错误：国家代码 '{code}' 已存在。"
        if transit_fee < 0:
            return False, "错误：过境费不能为负数。"
        self.countries[code_lower] = Country(name, code_lower, transit_fee)
        self.save_data()
        return True, f"成功添加国家：{name} ({code})，过境费 {transit_fee:.2f}"

    def list_countries(self) -> List[Tuple[str, str, float]]:
        """返回 [(code, name, transit_fee), ...]"""
        return [(code, c.name, c.transit_fee) for code, c in self.countries.items()]

    def modify_country(self, code: str, new_name: str = None, new_fee: float = None) -> Tuple[bool, str]:
        code_lower = code.lower()
        if code_lower not in self.countries:
            return False, f"错误：国家 '{code}' 不存在。"
        c = self.countries[code_lower]
        if new_name:
            c.name = new_name
        if new_fee is not None:
            if new_fee < 0:
                return False, "错误：过境费不能为负数。"
            c.transit_fee = new_fee
        self.save_data()
        return True, f"成功修改国家 {code}。"

    def delete_country(self, code: str) -> Tuple[bool, str]:
        code_lower = code.lower()
        if code_lower not in self.countries:
            return False, f"错误：国家 '{code}' 不存在。"
        # 删除生产记录
        to_delete_prod = [k for k in self.production_prices if k[0] == code_lower]
        for k in to_delete_prod:
            del self.production_prices[k]
        # 删除关税记录
        to_delete_tariff = [k for k in self.tariffs if k[0] == code_lower or k[1] == code_lower]
        for k in to_delete_tariff:
            del self.tariffs[k]
        del self.countries[code_lower]
        self.save_data()
        return True, f"成功删除国家 {code} 及其关联数据。"

    # ------------------- 商品管理 -------------------
    def add_commodity(self, name: str) -> Tuple[bool, str]:
        name_lower = name.lower()
        if name_lower in self.commodities:
            return False, f"错误：商品 '{name}' 已存在。"
        self.commodities[name_lower] = Commodity(name_lower)
        self.save_data()
        return True, f"成功添加商品：{name}"

    def set_production_price(self, country_code: str, commodity_name: str, price: float) -> Tuple[bool, str]:
        c_code = country_code.lower()
        c_name = commodity_name.lower()
        if c_code not in self.countries:
            return False, f"错误：国家 '{country_code}' 不存在。"
        if c_name not in self.commodities:
            return False, f"错误：商品 '{commodity_name}' 不存在。"
        if price < 0:
            return False, "错误：出厂价格不能为负数。"
        self.production_prices[(c_code, c_name)] = price
        self.save_data()
        return True, f"成功设置 {country_code} 生产 {commodity_name}，出厂价 {price:.2f}"

    def cancel_production(self, country_code: str, commodity_name: str) -> Tuple[bool, str]:
        c_code = country_code.lower()
        c_name = commodity_name.lower()
        if (c_code, c_name) in self.production_prices:
            del self.production_prices[(c_code, c_name)]
            self.save_data()
            return True, f"已取消 {country_code} 对 {commodity_name} 的生产。"
        else:
            return False, f"{country_code} 原本就不生产 {commodity_name}。"

    def list_commodities(self) -> Dict[str, Dict[str, float]]:
        """返回 {商品名: {国家代码: 出厂价}}"""
        result = {}
        for comm in self.commodities.values():
            result[comm.name] = {}
        for (c_code, c_name), price in self.production_prices.items():
            result[c_name][c_code] = price
        return result

    def delete_commodity(self, commodity_name: str) -> Tuple[bool, str]:
        name_lower = commodity_name.lower()
        if name_lower not in self.commodities:
            return False, f"错误：商品 '{commodity_name}' 不存在。"
        # 删除生产记录
        to_delete_prod = [k for k in self.production_prices if k[1] == name_lower]
        for k in to_delete_prod:
            del self.production_prices[k]
        # 删除关税记录
        to_delete_tariff = [k for k in self.tariffs if k[2] == name_lower]
        for k in to_delete_tariff:
            del self.tariffs[k]
        del self.commodities[name_lower]
        self.save_data()
        return True, f"成功删除商品 {commodity_name} 及其关联数据。"

    # ------------------- 关税管理 -------------------
    def set_tariff(self, export_code: str, import_code: str, commodity_name: str, rate: float, record_history: bool = True) -> Tuple[bool, str]:
        e_code = export_code.lower()
        i_code = import_code.lower()
        c_name = commodity_name.lower()
        if e_code not in self.countries or i_code not in self.countries:
            return False, "错误：出口国或进口国不存在。"
        if c_name not in self.commodities:
            return False, f"错误：商品 '{commodity_name}' 不存在。"
        if e_code == i_code:
            return False, "错误：出口国与进口国不能相同。"
        if rate < 0:
            return False, "错误：关税税率不能为负数。"
        
        key = (e_code, i_code, c_name)
        
        # 记录历史以支持撤销功能
        old_rate = self.tariffs.get(key, 0.0)
        if not self._batch_mode and record_history and old_rate != rate:
            self.history.append({'key': key, 'old_rate': old_rate})
        elif self._batch_mode and old_rate != rate:
            self._batch_changes.append({'key': key, 'old_rate': old_rate})
        
        # 执行关税修改        
        if rate == 0:
            # 如果税率为 0，则尝试从字典中移除该条目
            self.tariffs.pop(key, None)
            msg = f"已移除关税记录（设为 0%）：{export_code} -> {import_code}，{commodity_name}"
        else:
            self.tariffs[key] = rate
            msg = f"成功设置关税：{export_code} -> {import_code}，{commodity_name}，税率 {rate:.2f}%"

        self.save_data()
        return True, msg

    def get_tariff(self, export_code: str, import_code: str, commodity_name: str) -> float:
        key = (export_code.lower(), import_code.lower(), commodity_name.lower())
        return self.tariffs.get(key, 0.0)

    def get_tariff_matrix(self, commodity_name: str) -> Tuple[List[str], List[List[str]]]:
        """返回 (国家代码列表, 矩阵数据) 矩阵元素为字符串如'25.00%'或'-'"""
        c_name = commodity_name.lower()
        if c_name not in self.commodities:
            return [], []
        codes = list(self.countries.keys())
        n = len(codes)
        matrix = [[""] * n for _ in range(n)]
        for i, ex in enumerate(codes):
            for j, im in enumerate(codes):
                if ex == im:
                    matrix[i][j] = "-"
                else:
                    rate = self.get_tariff(ex, im, c_name)
                    matrix[i][j] = f"{rate:.2f}%"
        return codes, matrix

    # ------------------- 价格计算 -------------------
    def compute_path_price(self, commodity_name: str, path: List[str]) -> Tuple[float, List[float], str]:
        """返回 (最终价格, 中间价格列表, 错误信息) 成功时错误信息为空"""
        c_name = commodity_name.lower()
        if c_name not in self.commodities:
            return 0.0, [], f"商品 {commodity_name} 不存在"
        if len(path) < 1:
            return 0.0, [], "路径至少需要一个国家"
        path_lower = [p.lower() for p in path]
        if len(set(path_lower)) != len(path_lower):
            return 0.0, [], "路径中存在重复国家"
        for code in path_lower:
            if code not in self.countries:
                return 0.0, [], f"国家 {code} 不存在"
        start = path_lower[0]
        if (start, c_name) not in self.production_prices:
            return 0.0, [], f"起始国 {start} 不生产商品 {commodity_name}"
        price = self.production_prices[(start, c_name)]
        intermediate = [price]
        for i in range(1, len(path_lower)):
            export = path_lower[i-1]
            import_c = path_lower[i]
            tariff = self.get_tariff(export, import_c, c_name)
            price = price * (1 + tariff / 100.0) + self.countries[import_c].transit_fee
            intermediate.append(price)
        return price, intermediate, ""

    # ------------------- 最优采购路径（≤max_hops）-------------------
    def find_best_path(self, dest_country: str, commodity_name: str, max_hops: int = 4) -> Tuple[Optional[List[str]], float, str]:
        """使用改进版 Dijkstra (Uniform Cost Search) 算法查找最优采购路径"""
        if max_hops < 0:
            return None, 0.0, "错误：最大跳数不能为负数"

        dest = dest_country.lower()
        c_name = commodity_name.lower()

        if dest not in self.countries:
            return None, 0.0, f"目的国 {dest_country} 不存在"
        if c_name not in self.commodities:
            return None, 0.0, f"商品 {commodity_name} 不存在"

        # 优先队列，元素结构: (当前总价格, 跳数, 当前国家, 路径列表)
        # heapq 默认按元组的第一个元素（价格）作为优先级弹出
        pq = []

        # 帕累托(Pareto)剪枝记录: 记录到达每个国家的 (价格, 跳数) 历史
        # 如果新路径的价格更高且跳数更多（或相等），则该路径被"支配"，可以直接丢弃
        best_states = {code: [] for code in self.countries}

        producers = [code for code in self.countries if (code, c_name) in self.production_prices]
        if not producers:
            return None, 0.0, "未找到可行路径（可能没有国家生产该商品）"

        # 多源最短路初始化：将所有生产国作为起跑线，同时推入优先队列
        for prod in producers:
            start_price = self.production_prices[(prod, c_name)]
            heapq.heappush(pq, (start_price, 0, prod, [prod]))

        while pq:
            current_price, hops, curr_country, path = heapq.heappop(pq)

            # 1. 终点检查：由于是优先队列，第一个到达目的国的必定是全局最低价
            if curr_country == dest:
                return path, current_price, ""

            # 2. 剪枝判断：检查当前路径是否被更优的历史路径支配
            is_dominated = False
            for known_price, known_hops in best_states[curr_country]:
                if known_price <= current_price + 1e-9 and known_hops <= hops:
                    is_dominated = True
                    break
            
            if is_dominated:
                continue
            
            # 记录当前有效状态
            best_states[curr_country].append((current_price, hops))

            # 3. 跳数限制检查
            if hops >= max_hops:
                continue

            # 4. 扩展相邻国家
            for nxt_country, nxt_info in self.countries.items():
                if nxt_country in path: # 防止路径回环
                    continue
                
                tariff = self.get_tariff(curr_country, nxt_country, c_name)
                next_price = current_price * (1 + tariff / 100.0) + nxt_info.transit_fee
                
                heapq.heappush(pq, (next_price, hops + 1, nxt_country, path + [nxt_country]))

        return None, 0.0, "在给定的跳数限制内未找到可行路径"

    # ------------------- 制造业回流分析 -------------------
    def manufacturing_reshoring(self, home_country: str, commodity_name: str) -> Tuple[Dict[str, Tuple[float, float]], str, float]:
        """
        制造业回流分析（改进版）：利用 Dijkstra 算法计算保护本国产业的最低关税。
        返回: (result, error_msg, home_price)
        """
        home = home_country.lower()
        c_name = commodity_name.lower()
        
        if home not in self.countries:
            return {}, f"本国 {home_country} 不存在", 0.0
        if (home, c_name) not in self.production_prices:
            return {}, f"本国 {home_country} 不生产 {commodity_name}，无法分析", 0.0

        home_price = self.production_prices[(home, c_name)]
        home_fee = self.countries[home].transit_fee
        max_hops_to_x = 3

        result = {}

        for x_code in self.countries:
            if x_code == home:
                continue

            current_rate = self.get_tariff(x_code, home, c_name)

            # 核心改进：直接复用已优化的 Dijkstra 算法，寻找商品到达竞争国 X 的全局最低价
            # 因为最低的进口价（min_price）必定对应着所需的最高保护关税
            path, best_price_at_x, err = self.find_best_path(x_code, c_name, max_hops=max_hops_to_x)
            
            # 如果没有任何路径可以到达 X，说明 X 无法向本国出口该商品
            if not path or best_price_at_x <= 0:
                result[x_code] = ("不可能满足", current_rate)
                continue

            # 计算平衡关税
            # 目标公式：best_price_at_x * (1 + required_rate / 100) + home_fee >= home_price
            target_import_price = home_price - home_fee
            
            if target_import_price <= 0:
                # 逻辑保护：如果本国出厂价极低（甚至低于自己的过境费），说明绝对具备全球竞争优势，无需关税保护
                required = 0.0
            else:
                required = (target_import_price / best_price_at_x - 1.0) * 100.0

            if required > 10000:  # 设定一个合理的上限，防止因为极端低价导致的数值爆炸
                result[x_code] = ("不可能满足", current_rate)
            else:
                new_rate = max(current_rate, required, 0.0)
                result[x_code] = (new_rate, current_rate)

        return result, "", home_price
    
    # ==================== 撤销功能 ====================
    def undo_tariff_change(self) -> Tuple[bool, str]:
        if not self.history:
            return False, "暂无关税修改历史可回滚"

        last = self.history.pop()
        if 'batch' in last and last['batch']:
            # 批量撤销
            changes = last['changes']
            for change in changes:
                key = change['key']
                old_val = change['old_rate']
                if old_val == 0:
                    self.tariffs.pop(key, None)
                else:
                    self.tariffs[key] = old_val
            self.save_data()
            return True, f"已撤销批量操作，共恢复 {len(changes)} 项关税设置。"
        else:
            # 单次撤销（原有逻辑）
            key = last['key']
            old_val = last['old_rate']
            if old_val == 0:
                self.tariffs.pop(key, None)
            else:
                self.tariffs[key] = old_val
            self.save_data()
            return True, f"已撤销最后一次操作。{key[0]} -> {key[1]} ({key[2]}) 关税已恢复为 {old_val}%"
    
    def begin_batch(self):
        """开始批量操作，后续的 set_tariff 不会单独记录历史"""
        self._batch_mode = True
        self._batch_changes = []

    def commit_batch(self):
        """提交批量操作，将本次所有变更合并为一条历史记录"""
        if self._batch_changes:
            self.history.append({
                'batch': True,
                'changes': self._batch_changes.copy()
            })
        self._batch_mode = False
        self._batch_changes = []

    def cancel_batch(self):
        """取消批量操作，丢弃暂存的变更"""
        self._batch_mode = False
        self._batch_changes = []
  
    # ==================== 数据导入导出功能 ====================    
    def export_data(self, target_filepath: str) -> Tuple[bool, str]:
        """
        将当前内存中的数据导出为外部 JSON 文件。
        """
        try:
            # 复用你原来的数据序列化逻辑
            data = {
                "countries": {code: asdict(c) for code, c in self.countries.items()},
                "commodities": {name: asdict(c) for name, c in self.commodities.items()},
                "production_prices": {f"{k[0]}||{k[1]}": v for k, v in self.production_prices.items()},
                "tariffs": {f"{k[0]}||{k[1]}||{k[2]}": v for k, v in self.tariffs.items()}
            }
            with open(target_filepath, 'w', encoding='utf-8') as f:
                json.dump(data, f, ensure_ascii=False, indent=4)
            return True, f"数据已成功导出至: {target_filepath}"
        except Exception as e:
            return False, f"导出失败: {str(e)}"

    def import_data(self, source_filepath: str) -> Tuple[bool, str]:
        """
        从外部 JSON 文件导入数据，覆盖当前系统。
        """
        if not os.path.exists(source_filepath):
            return False, f"未找到文件: {source_filepath}"
        
        # 巧妙复用已有的 load_data：临时切换 data_file 指针
        original_file = self.data_file
        try:
            self.data_file = source_filepath
            self.load_data()           # 读取外部数据到内存
            self.data_file = original_file # 切回默认的配置文件名
            self.save_data()           # 将新读入的数据持久化到默认配置中
            self.history.clear()       # 导入新数据后清空旧的历史记录
            return True, f"已成功从 {source_filepath} 导入数据"
        except Exception as e:
            # 如果出错，恢复原来的文件指针
            self.data_file = original_file
            return False, f"导入数据时格式错误或发生异常: {str(e)}"


# ==================== 图形界面部分 ====================
class TariffGUI(tk.Tk):
    """基于 tkinter 的图形界面"""

    def __init__(self):
        super().__init__()
        self.title("全球关税模拟系统")
        self.geometry("900x700")
        self.system = TariffSystem()
        self.create_widgets()

    def create_widgets(self):
        # 创建选项卡（Notebook）
        self.notebook = ttk.Notebook(self)
        self.notebook.pack(fill=tk.BOTH, expand=True)

        # 各功能页
        self.country_frame = ttk.Frame(self.notebook)
        self.commodity_frame = ttk.Frame(self.notebook)
        self.tariff_frame = ttk.Frame(self.notebook)
        self.path_frame = ttk.Frame(self.notebook)
        self.best_path_frame = ttk.Frame(self.notebook)
        self.reshoring_frame = ttk.Frame(self.notebook)

        self.notebook.add(self.country_frame, text="国家管理")
        self.notebook.add(self.commodity_frame, text="商品管理")
        self.notebook.add(self.tariff_frame, text="关税管理")
        self.notebook.add(self.path_frame, text="路径价格计算")
        self.notebook.add(self.best_path_frame, text="最优采购路径")
        self.notebook.add(self.reshoring_frame, text="制造业回流分析")

        self._build_country_tab()
        self._build_commodity_tab()
        self._build_tariff_tab()
        self._build_path_tab()
        self._build_best_path_tab()
        self._build_reshoring_tab()

        # 底部状态栏
        self.status_var = tk.StringVar()
        self.status_bar = ttk.Label(self, textvariable=self.status_var, relief=tk.SUNKEN, anchor=tk.W)
        self.status_bar.pack(side=tk.BOTTOM, fill=tk.X)
        self.update_status("就绪")
        
        self.notebook.bind("<<NotebookTabChanged>>", self._on_tab_changed)

    def update_status(self, msg):
        self.status_var.set(msg)
        
    def _on_tab_changed(self, event):
        """当切换选项卡时，刷新所有视图以确保数据同步"""
        self.refresh_country_list()
        self.refresh_commodity_list()

    # ------------------- 国家管理 -------------------
    def _build_country_tab(self):
        # 左侧：添加/修改/删除 表单
        frame_left = ttk.LabelFrame(self.country_frame, text="操作")
        frame_left.pack(side=tk.LEFT, fill=tk.Y, padx=5, pady=5)

        ttk.Label(frame_left, text="代码:").grid(row=0, column=0, sticky=tk.W, padx=5, pady=2)
        self.country_code_entry = ttk.Entry(frame_left, width=15)
        self.country_code_entry.grid(row=0, column=1, padx=5, pady=2)

        ttk.Label(frame_left, text="名称:").grid(row=1, column=0, sticky=tk.W, padx=5, pady=2)
        self.country_name_entry = ttk.Entry(frame_left, width=15)
        self.country_name_entry.grid(row=1, column=1, padx=5, pady=2)

        ttk.Label(frame_left, text="过境费:").grid(row=2, column=0, sticky=tk.W, padx=5, pady=2)
        self.country_fee_entry = ttk.Entry(frame_left, width=15)
        self.country_fee_entry.grid(row=2, column=1, padx=5, pady=2)

        btn_add = ttk.Button(frame_left, text="添加国家", command=self.add_country)
        btn_add.grid(row=3, column=0, columnspan=2, pady=5)
        btn_modify = ttk.Button(frame_left, text="修改国家", command=self.modify_country)
        btn_modify.grid(row=4, column=0, columnspan=2, pady=5)
        btn_delete = ttk.Button(frame_left, text="删除国家", command=self.delete_country)
        btn_delete.grid(row=5, column=0, columnspan=2, pady=5)

        # 右侧：国家列表
        frame_right = ttk.LabelFrame(self.country_frame, text="国家列表")
        frame_right.pack(side=tk.RIGHT, fill=tk.BOTH, expand=True, padx=5, pady=5)

        columns = ("code", "name", "fee")
        self.country_tree = ttk.Treeview(frame_right, columns=columns, show="headings")
        self.country_tree.heading("code", text="代码")
        self.country_tree.heading("name", text="名称")
        self.country_tree.heading("fee", text="过境费")
        self.country_tree.column("code", width=80)
        self.country_tree.column("name", width=120)
        self.country_tree.column("fee", width=80)

        scrollbar = ttk.Scrollbar(frame_right, orient=tk.VERTICAL, command=self.country_tree.yview)
        self.country_tree.configure(yscrollcommand=scrollbar.set)
        scrollbar.pack(side=tk.RIGHT, fill=tk.Y)
        self.country_tree.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)

        self.refresh_country_list()
        # 绑定选中事件
        self.country_tree.bind("<<TreeviewSelect>>", self.on_country_select)

    def refresh_country_list(self):
        for item in self.country_tree.get_children():
            self.country_tree.delete(item)
        for code, name, fee in self.system.list_countries():
            self.country_tree.insert("", tk.END, values=(code, name, f"{fee:.2f}"))

    def on_country_select(self, event):
        selected = self.country_tree.selection()
        if selected:
            values = self.country_tree.item(selected[0], "values")
            if values:
                self.country_code_entry.delete(0, tk.END)
                self.country_code_entry.insert(0, values[0])
                self.country_name_entry.delete(0, tk.END)
                self.country_name_entry.insert(0, values[1])
                self.country_fee_entry.delete(0, tk.END)
                self.country_fee_entry.insert(0, values[2])

    def add_country(self):
        code = self.country_code_entry.get().strip()
        name = self.country_name_entry.get().strip()
        fee_str = self.country_fee_entry.get().strip()
        if not code or not name or not fee_str:
            messagebox.showerror("错误", "请填写完整信息")
            return
        try:
            fee = float(fee_str)
        except ValueError:
            messagebox.showerror("错误", "过境费必须是数字")
            return
        ok, msg = self.system.add_country(name, code, fee)
        if ok:
            self.refresh_country_list()
            self.update_status(msg)
            messagebox.showinfo("成功", msg)
        else:
            messagebox.showerror("错误", msg)

    def modify_country(self):
        code = self.country_code_entry.get().strip()
        if not code:
            messagebox.showerror("错误", "请选择或输入要修改的国家代码")
            return
        new_name = self.country_name_entry.get().strip()
        fee_str = self.country_fee_entry.get().strip()
        new_fee = None
        if fee_str:
            try:
                new_fee = float(fee_str)
            except ValueError:
                messagebox.showerror("错误", "过境费必须是数字")
                return
        ok, msg = self.system.modify_country(code, new_name if new_name else None, new_fee)
        if ok:
            self.refresh_country_list()
            self.update_status(msg)
            messagebox.showinfo("成功", msg)
        else:
            messagebox.showerror("错误", msg)

    def delete_country(self):
        code = self.country_code_entry.get().strip()
        if not code:
            messagebox.showerror("错误", "请选择或输入要删除的国家代码")
            return
        if messagebox.askyesno("确认", f"确定要删除国家 {code} 及其所有关联数据吗？"):
            ok, msg = self.system.delete_country(code)
            if ok:
                self.refresh_country_list()
                self.update_status(msg)
                messagebox.showinfo("成功", msg)
            else:
                messagebox.showerror("错误", msg)

    # ------------------- 商品管理 -------------------
    def _build_commodity_tab(self):
        # 操作区
        frame_op = ttk.LabelFrame(self.commodity_frame, text="商品操作")
        frame_op.pack(side=tk.LEFT, fill=tk.Y, padx=5, pady=5)

        ttk.Label(frame_op, text="商品名称:").grid(row=0, column=0, padx=5, pady=2)
        self.comm_name_entry = ttk.Entry(frame_op, width=15)
        self.comm_name_entry.grid(row=0, column=1, padx=5, pady=2)
        btn_add_comm = ttk.Button(frame_op, text="添加商品", command=self.add_commodity)
        btn_add_comm.grid(row=1, column=0, columnspan=2, pady=5)
        btn_del_comm = ttk.Button(frame_op, text="删除商品", command=self.delete_commodity)
        btn_del_comm.grid(row=2, column=0, columnspan=2, pady=5)

        ttk.Separator(frame_op, orient=tk.HORIZONTAL).grid(row=3, column=0, columnspan=2, sticky=tk.EW, pady=10)

        ttk.Label(frame_op, text="设置/取消生产:").grid(row=4, column=0, columnspan=2, pady=5)
        ttk.Label(frame_op, text="国家代码:").grid(row=5, column=0, padx=5, pady=2)
        self.prod_country_entry = ttk.Entry(frame_op, width=15)
        self.prod_country_entry.grid(row=5, column=1, padx=5, pady=2)
        ttk.Label(frame_op, text="商品名称:").grid(row=6, column=0, padx=5, pady=2)
        self.prod_comm_entry = ttk.Entry(frame_op, width=15)
        self.prod_comm_entry.grid(row=6, column=1, padx=5, pady=2)
        ttk.Label(frame_op, text="出厂价格:").grid(row=7, column=0, padx=5, pady=2)
        self.prod_price_entry = ttk.Entry(frame_op, width=15)
        self.prod_price_entry.grid(row=7, column=1, padx=5, pady=2)
        btn_set_prod = ttk.Button(frame_op, text="设置生产价格", command=self.set_production)
        btn_set_prod.grid(row=8, column=0, columnspan=2, pady=5)
        btn_cancel_prod = ttk.Button(frame_op, text="取消生产", command=self.cancel_production)
        btn_cancel_prod.grid(row=9, column=0, columnspan=2, pady=5)

        # 显示区
        frame_disp = ttk.LabelFrame(self.commodity_frame, text="商品及生产情况")
        frame_disp.pack(side=tk.RIGHT, fill=tk.BOTH, expand=True, padx=5, pady=5)

        self.comm_tree = ttk.Treeview(frame_disp, show="tree")
        scrollbar = ttk.Scrollbar(frame_disp, orient=tk.VERTICAL, command=self.comm_tree.yview)
        self.comm_tree.configure(yscrollcommand=scrollbar.set)
        scrollbar.pack(side=tk.RIGHT, fill=tk.Y)
        self.comm_tree.pack(fill=tk.BOTH, expand=True)

        self.refresh_commodity_list()

    def refresh_commodity_list(self):
        for item in self.comm_tree.get_children():
            self.comm_tree.delete(item)
        data = self.system.list_commodities()
        for comm, prod_dict in data.items():
            node = self.comm_tree.insert("", tk.END, text=comm, open=True)
            for code, price in prod_dict.items():
                self.comm_tree.insert(node, tk.END, text=f"{code}: {price:.2f}")

    def add_commodity(self):
        name = self.comm_name_entry.get().strip()
        if not name:
            messagebox.showerror("错误", "请输入商品名称")
            return
        ok, msg = self.system.add_commodity(name)
        if ok:
            self.refresh_commodity_list()
            self.update_status(msg)
            messagebox.showinfo("成功", msg)
        else:
            messagebox.showerror("错误", msg)

    def delete_commodity(self):
        name = self.comm_name_entry.get().strip()
        if not name:
            messagebox.showerror("错误", "请输入要删除的商品名称")
            return
        if messagebox.askyesno("确认", f"确定删除商品 {name} 及其所有关联数据吗？"):
            ok, msg = self.system.delete_commodity(name)
            if ok:
                self.refresh_commodity_list()
                self.update_status(msg)
                messagebox.showinfo("成功", msg)
            else:
                messagebox.showerror("错误", msg)

    def set_production(self):
        country = self.prod_country_entry.get().strip()
        comm = self.prod_comm_entry.get().strip()
        price_str = self.prod_price_entry.get().strip()
        if not country or not comm or not price_str:
            messagebox.showerror("错误", "请填写完整信息")
            return
        try:
            price = float(price_str)
        except ValueError:
            messagebox.showerror("错误", "价格必须是数字")
            return
        ok, msg = self.system.set_production_price(country, comm, price)
        if ok:
            self.refresh_commodity_list()
            self.update_status(msg)
            messagebox.showinfo("成功", msg)
        else:
            messagebox.showerror("错误", msg)

    def cancel_production(self):
        country = self.prod_country_entry.get().strip()
        comm = self.prod_comm_entry.get().strip()
        if not country or not comm:
            messagebox.showerror("错误", "请填写国家代码和商品名称")
            return
        ok, msg = self.system.cancel_production(country, comm)
        if ok:
            self.refresh_commodity_list()
            self.update_status(msg)
            messagebox.showinfo("成功", msg)
        else:
            messagebox.showerror("错误", msg)

    # ------------------- 关税管理 -------------------
    def _build_tariff_tab(self):
        # 设置关税
        frame_set = ttk.LabelFrame(self.tariff_frame, text="设置关税")
        frame_set.pack(side=tk.TOP, fill=tk.X, padx=5, pady=5)

        ttk.Label(frame_set, text="出口国:").grid(row=0, column=0, padx=5, pady=2)
        self.tariff_export_entry = ttk.Entry(frame_set, width=10)
        self.tariff_export_entry.grid(row=0, column=1, padx=5, pady=2)
        ttk.Label(frame_set, text="进口国:").grid(row=0, column=2, padx=5, pady=2)
        self.tariff_import_entry = ttk.Entry(frame_set, width=10)
        self.tariff_import_entry.grid(row=0, column=3, padx=5, pady=2)
        ttk.Label(frame_set, text="商品:").grid(row=0, column=4, padx=5, pady=2)
        self.tariff_comm_entry = ttk.Entry(frame_set, width=10)
        self.tariff_comm_entry.grid(row=0, column=5, padx=5, pady=2)
        ttk.Label(frame_set, text="税率(%):").grid(row=0, column=6, padx=5, pady=2)
        self.tariff_rate_entry = ttk.Entry(frame_set, width=8)
        self.tariff_rate_entry.grid(row=0, column=7, padx=5, pady=2)
        btn_set_tariff = ttk.Button(frame_set, text="设置", command=self.set_tariff)
        btn_set_tariff.grid(row=0, column=8, padx=5, pady=2)

        # 查看关税
        frame_view = ttk.LabelFrame(self.tariff_frame, text="查看关税")
        frame_view.pack(side=tk.TOP, fill=tk.X, padx=5, pady=5)
        ttk.Label(frame_view, text="出口国:").grid(row=0, column=0, padx=5, pady=2)
        self.view_export_entry = ttk.Entry(frame_view, width=10)
        self.view_export_entry.grid(row=0, column=1, padx=5, pady=2)
        ttk.Label(frame_view, text="进口国:").grid(row=0, column=2, padx=5, pady=2)
        self.view_import_entry = ttk.Entry(frame_view, width=10)
        self.view_import_entry.grid(row=0, column=3, padx=5, pady=2)
        ttk.Label(frame_view, text="商品:").grid(row=0, column=4, padx=5, pady=2)
        self.view_comm_entry = ttk.Entry(frame_view, width=10)
        self.view_comm_entry.grid(row=0, column=5, padx=5, pady=2)
        btn_view = ttk.Button(frame_view, text="查询", command=self.view_tariff)
        btn_view.grid(row=0, column=6, padx=5, pady=2)
        self.view_result_label = ttk.Label(frame_view, text="")
        self.view_result_label.grid(row=0, column=7, padx=5, pady=2)

        # --- 下方：关税矩阵展示区 ---
        frame_matrix = ttk.LabelFrame(self.tariff_frame, text="关税矩阵")
        frame_matrix.pack(fill=tk.BOTH, expand=True, padx=5, pady=5)

        # 商品名称查询行
        query_frame = ttk.Frame(frame_matrix)
        query_frame.pack(fill=tk.X, padx=5, pady=5)
        ttk.Label(query_frame, text="商品名称:").pack(side=tk.LEFT)
        self.matrix_comm_entry = ttk.Entry(query_frame, width=15)
        self.matrix_comm_entry.pack(side=tk.LEFT, padx=5)
        ttk.Button(query_frame, text="显示矩阵", command=self.show_tariff_matrix).pack(side=tk.LEFT)
        
        # --- 核心修复部分：添加水平和垂直滚动条 ---
        # 创建一个容器来放置 Text 和垂直滚动条
        text_container = ttk.Frame(frame_matrix)
        text_container.pack(fill=tk.BOTH, expand=True)

        # 垂直滚动条
        v_scroll = ttk.Scrollbar(text_container, orient=tk.VERTICAL)
        v_scroll.pack(side=tk.RIGHT, fill=tk.Y)

        # 水平滚动条 (放在容器下方)
        h_scroll = ttk.Scrollbar(frame_matrix, orient=tk.HORIZONTAL)
        h_scroll.pack(side=tk.BOTTOM, fill=tk.X)

        # Text 组件：必须设置 wrap=tk.NONE 才能保持列不重叠
        self.matrix_text = tk.Text(
            text_container, 
            height=20, 
            font=("Courier New", 10), 
            wrap=tk.NONE,  # 关键：禁止自动换行
            xscrollcommand=h_scroll.set,
            yscrollcommand=v_scroll.set
        )
        self.matrix_text.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)

        # 绑定滚动条功能
        v_scroll.config(command=self.matrix_text.yview)
        h_scroll.config(command=self.matrix_text.xview)
        
        # ---------- 新增：系统工具区域 ----------
        frame_tools = ttk.LabelFrame(self.tariff_frame, text="系统工具")
        frame_tools.pack(side=tk.BOTTOM, fill=tk.X, padx=5, pady=5)

        btn_export = ttk.Button(frame_tools, text="导出数据", command=self.export_data)
        btn_export.pack(side=tk.LEFT, padx=5, pady=5)

        btn_import = ttk.Button(frame_tools, text="导入数据", command=self.import_data)
        btn_import.pack(side=tk.LEFT, padx=5, pady=5)

        btn_undo = ttk.Button(frame_tools, text="撤销关税修改", command=self.undo_tariff)
        btn_undo.pack(side=tk.LEFT, padx=5, pady=5)

    def set_tariff(self):
        ex = self.tariff_export_entry.get().strip()
        im = self.tariff_import_entry.get().strip()
        comm = self.tariff_comm_entry.get().strip()
        rate_str = self.tariff_rate_entry.get().strip()
        if not ex or not im or not comm or not rate_str:
            messagebox.showerror("错误", "请填写完整信息")
            return
        try:
            rate = float(rate_str)
        except ValueError:
            messagebox.showerror("错误", "税率必须是数字")
            return
        ok, msg = self.system.set_tariff(ex, im, comm, rate)
        if ok:
            self.update_status(msg)
            messagebox.showinfo("成功", msg)
        else:
            messagebox.showerror("错误", msg)

    def view_tariff(self):
        ex = self.view_export_entry.get().strip()
        im = self.view_import_entry.get().strip()
        comm = self.view_comm_entry.get().strip()
        if not ex or not im or not comm:
            messagebox.showerror("错误", "请填写出口国、进口国和商品")
            return
        rate = self.system.get_tariff(ex, im, comm)
        self.view_result_label.config(text=f"{rate:.2f}%")
        self.update_status(f"查询 {ex}->{im} {comm} 关税: {rate:.2f}%")

    def show_tariff_matrix(self):
        comm = self.matrix_comm_entry.get().strip()
        if not comm:
            messagebox.showerror("错误", "请输入商品名称")
            return
        codes, matrix = self.system.get_tariff_matrix(comm)
        if not codes:
            messagebox.showerror("错误", f"商品 {comm} 不存在")
            return
        self.matrix_text.delete(1.0, tk.END)
        # 构建表头
        header = "     " + "".join(f"{c:>8}" for c in codes)
        self.matrix_text.insert(tk.END, header + "\n")
        for i, ex in enumerate(codes):
            row = f"{ex:<5}" + "".join(f"{matrix[i][j]:>8}" for j in range(len(codes)))
            self.matrix_text.insert(tk.END, row + "\n")
        self.update_status(f"显示 {comm} 关税矩阵")
        
    def export_data(self):
        from tkinter import filedialog
        filepath = filedialog.asksaveasfilename(defaultextension=".json", filetypes=[("JSON files", "*.json")])
        if not filepath:
            return
        ok, msg = self.system.export_data(filepath)
        if ok:
            messagebox.showinfo("成功", msg)
            self.update_status(msg)
        else:
            messagebox.showerror("错误", msg)

    def import_data(self):
        from tkinter import filedialog
        filepath = filedialog.askopenfilename(filetypes=[("JSON files", "*.json")])
        if not filepath:
            return
        if messagebox.askyesno("确认", "导入数据将覆盖当前系统所有内容，是否继续？"):
            ok, msg = self.system.import_data(filepath)
            if ok:
                # 刷新所有视图
                self.refresh_country_list()
                self.refresh_commodity_list()
                # 清除关税矩阵显示区
                if hasattr(self, 'matrix_text'):
                    self.matrix_text.delete(1.0, tk.END)
                # 清空各输入框（可选）
                self.update_status(msg)
                messagebox.showinfo("成功", msg)
            else:
                messagebox.showerror("错误", msg)

    def undo_tariff(self):
        ok, msg = self.system.undo_tariff_change()
        if ok:
            self.update_status(msg)
            messagebox.showinfo("成功", msg)
            # 刷新关税矩阵显示（如果有正在查看的商品）
            comm = self.matrix_comm_entry.get().strip()
            if comm:
                self.show_tariff_matrix()
        else:
            messagebox.showwarning("提示", msg)

    # ------------------- 路径价格计算 -------------------
    def _build_path_tab(self):
        frame = ttk.Frame(self.path_frame)
        frame.pack(fill=tk.BOTH, expand=True, padx=10, pady=10)

        ttk.Label(frame, text="商品名称:").grid(row=0, column=0, sticky=tk.W, pady=5)
        self.path_comm_entry = ttk.Entry(frame, width=20)
        self.path_comm_entry.grid(row=0, column=1, sticky=tk.W, pady=5)

        ttk.Label(frame, text="路径 (用 > 分隔):").grid(row=1, column=0, sticky=tk.W, pady=5)
        self.path_entry = ttk.Entry(frame, width=40)
        self.path_entry.grid(row=1, column=1, sticky=tk.W, pady=5)

        btn_calc = ttk.Button(frame, text="计算价格", command=self.compute_path)
        btn_calc.grid(row=2, column=0, columnspan=2, pady=10)

        self.path_result_text = scrolledtext.ScrolledText(frame, height=15)
        self.path_result_text.grid(row=3, column=0, columnspan=2, sticky=tk.NSEW, pady=5)
        frame.columnconfigure(1, weight=1)
        frame.rowconfigure(3, weight=1)

    def compute_path(self):
        comm = self.path_comm_entry.get().strip()
        path_str = self.path_entry.get().strip()
        if not comm or not path_str:
            messagebox.showerror("错误", "请填写商品名称和路径")
            return
        path = [p.strip() for p in path_str.split('>')]
        price, intermediates, err = self.system.compute_path_price(comm, path)
        if err:
            messagebox.showerror("错误", err)
            return
        self.path_result_text.delete(1.0, tk.END)
        self.path_result_text.insert(tk.END, f"商品: {comm}\n路径: {' -> '.join(path)}\n\n")
        for i, code in enumerate(path):
            self.path_result_text.insert(tk.END, f"到达 {code} 价格: {intermediates[i]:.2f}\n")
        self.path_result_text.insert(tk.END, f"\n最终价格: {price:.2f}\n")
        self.update_status("路径价格计算完成")

    # ------------------- 最优采购路径 -------------------
    def _build_best_path_tab(self):
        frame = ttk.Frame(self.best_path_frame)
        frame.pack(fill=tk.BOTH, expand=True, padx=10, pady=10)

        ttk.Label(frame, text="目的国代码:").grid(row=0, column=0, sticky=tk.W, pady=5)
        self.best_dest_entry = ttk.Entry(frame, width=20)
        self.best_dest_entry.grid(row=0, column=1, sticky=tk.W, pady=5)

        ttk.Label(frame, text="商品名称:").grid(row=1, column=0, sticky=tk.W, pady=5)
        self.best_comm_entry = ttk.Entry(frame, width=20)
        self.best_comm_entry.grid(row=1, column=1, sticky=tk.W, pady=5)

        ttk.Label(frame, text="最大跳数 (默认4):").grid(row=2, column=0, sticky=tk.W, pady=5)
        self.best_hops_entry = ttk.Entry(frame, width=10)
        self.best_hops_entry.insert(0, "4")
        self.best_hops_entry.grid(row=2, column=1, sticky=tk.W, pady=5)

        btn_best = ttk.Button(frame, text="查询最优路径", command=self.find_best_path)
        btn_best.grid(row=3, column=0, columnspan=2, pady=10)

        self.best_result_text = scrolledtext.ScrolledText(frame, height=15)
        self.best_result_text.grid(row=4, column=0, columnspan=2, sticky=tk.NSEW, pady=5)
        frame.columnconfigure(1, weight=1)
        frame.rowconfigure(4, weight=1)

    def find_best_path(self):
        dest = self.best_dest_entry.get().strip()
        comm = self.best_comm_entry.get().strip()
        hops_str = self.best_hops_entry.get().strip()
        if not dest or not comm:
            messagebox.showerror("错误", "请填写目的国和商品")
            return
        try:
            max_hops = int(hops_str) if hops_str else 4
        except ValueError:
            max_hops = 4
        path, price, err = self.system.find_best_path(dest, comm, max_hops)
        if err:
            messagebox.showerror("错误", err)
            return
        self.best_result_text.delete(1.0, tk.END)
        self.best_result_text.insert(tk.END, f"最优路径: {' -> '.join(path)}\n")
        self.best_result_text.insert(tk.END, f"最终价格: {price:.2f}\n")
        self.update_status("最优路径查询完成")

    # ------------------- 制造业回流分析 -------------------
    def _build_reshoring_tab(self):
        frame = ttk.Frame(self.reshoring_frame)
        frame.pack(fill=tk.BOTH, expand=True, padx=10, pady=10)

        ttk.Label(frame, text="本国代码:").grid(row=0, column=0, sticky=tk.W, pady=5)
        self.home_entry = ttk.Entry(frame, width=20)
        self.home_entry.grid(row=0, column=1, sticky=tk.W, pady=5)

        ttk.Label(frame, text="商品名称:").grid(row=1, column=0, sticky=tk.W, pady=5)
        self.reshore_comm_entry = ttk.Entry(frame, width=20)
        self.reshore_comm_entry.grid(row=1, column=1, sticky=tk.W, pady=5)

        btn_analyze = ttk.Button(frame, text="分析", command=self.reshoring_analysis)
        btn_analyze.grid(row=2, column=0, columnspan=2, pady=10)

        self.reshore_result_text = scrolledtext.ScrolledText(frame, height=20)
        self.reshore_result_text.grid(row=3, column=0, columnspan=2, sticky=tk.NSEW, pady=5)
        frame.columnconfigure(1, weight=1)
        frame.rowconfigure(3, weight=1)

    def reshoring_analysis(self):
        home = self.home_entry.get().strip()
        comm = self.reshore_comm_entry.get().strip()
        if not home or not comm:
            messagebox.showerror("错误", "请填写本国代码和商品名称")
            return
        result, err, home_price = self.system.manufacturing_reshoring(home, comm)
        if err:
            messagebox.showerror("错误", err)
            return
        self.reshore_result_text.delete(1.0, tk.END)
        self.reshore_result_text.insert(tk.END, f"本国: {home}  商品: {comm}  出厂价: {home_price:.2f}\n\n")
        self.reshore_result_text.insert(tk.END, "关税调整建议（仅提高直接进口关税）:\n")
        for x_code, (new, old) in result.items():
            if isinstance(new, str):
                self.reshore_result_text.insert(tk.END, f"{x_code} -> {home}: {new} (当前 {old:.2f}%)\n")
            else:
                if new > old + 1e-9:
                    self.reshore_result_text.insert(tk.END, f"{x_code} -> {home}: 从 {old:.2f}% 提高到 {new:.2f}%\n")
                else:
                    self.reshore_result_text.insert(tk.END, f"{x_code} -> {home}: 当前已足够 ({old:.2f}%)\n")
        # 可选应用按钮
        if messagebox.askyesno("应用调整", "是否将上述关税调整应用到系统中？"):
        # 收集需要提高的关税项
            changes = []
            for x_code, (new, old) in result.items():
                if not isinstance(new, str) and new > old + 1e-9:
                    changes.append((x_code, home, comm, new))
            if not changes:
                messagebox.showinfo("提示", "没有需要调整的关税。")
                return

            try:
                self.system.begin_batch()
                for ex, im, cm, rate in changes:
                    self.system.set_tariff(ex, im, cm, rate)
                self.system.commit_batch()
                self.update_status(f"已应用 {len(changes)} 项关税调整，可一键撤销。")
                messagebox.showinfo("完成", f"已更新 {len(changes)} 项关税。\n使用「撤销关税修改」可一键回滚。")
                # 刷新关税矩阵显示
                comm_disp = self.matrix_comm_entry.get().strip()
                if comm_disp:
                    self.show_tariff_matrix()
            except Exception as e:
                self.system.cancel_batch()
                messagebox.showerror("错误", f"应用调整时出错：{str(e)}")


# ==================== 命令行界面 ====================
class TariffCLI:
    """基于命令行的交互界面"""

    def __init__(self):
        self.system = TariffSystem()

    def run(self):
        while True:
            print("\n" + "=" * 50)
            print("全球关税模拟系统 (命令行版)")
            print("1. 国家管理")
            print("2. 商品管理")
            print("3. 关税管理")
            print("4. 指定路径价格计算")
            print("5. 最优采购路径查询")
            print("6. 制造业回流分析")
            print("7. 系统工具（导入/导出/撤销关税）")
            print("8. 退出程序")
            choice = input("请选择操作：").strip()
            if choice == '1':
                self.country_menu_cli()
            elif choice == '2':
                self.commodity_menu_cli()
            elif choice == '3':
                self.tariff_menu_cli()
            elif choice == '4':
                self.interactive_path_price_cli()
            elif choice == '5':
                self.interactive_best_path_cli()
            elif choice == '6':
                home = input("请输入本国代码：").strip()
                comm = input("请输入商品名称：").strip()
                self.manufacturing_reshoring_cli(home, comm)
            elif choice == '7':
                self.system_tools_cli()
            elif choice == '8':               
                print("感谢使用，再见！")
                break
            else:
                print("无效选项，请重新输入。")

    def country_menu_cli(self):
        while True:
            print("\n--- 国家管理 ---")
            print("1. 添加国家")
            print("2. 列出所有国家")
            print("3. 修改国家")
            print("4. 删除国家")
            print("5. 返回上级菜单")
            sub = input("请选择：").strip()
            if sub == '1':
                name = input("名称：").strip()
                code = input("英文代码：").strip()
                try:
                    fee = float(input("过境费：").strip())
                    ok, msg = self.system.add_country(name, code, fee)
                    print(msg)
                except ValueError:
                    print("过境费必须是数字。")
            elif sub == '2':
                countries = self.system.list_countries()
                if not countries:
                    print("暂无国家。")
                else:
                    print(f"{'代码':<6} {'名称':<10} {'过境费':<10}")
                    for code, name, fee in countries:
                        print(f"{code:<6} {name:<10} {fee:<10.2f}")
            elif sub == '3':
                code = input("国家代码：").strip()
                new_name = input("新名称（留空不变）：").strip() or None
                fee_str = input("新过境费（留空不变）：").strip()
                new_fee = float(fee_str) if fee_str else None
                ok, msg = self.system.modify_country(code, new_name, new_fee)
                print(msg)
            elif sub == '4':
                code = input("国家代码：").strip()
                ok, msg = self.system.delete_country(code)
                print(msg)
            elif sub == '5':
                break
            else:
                print("无效选项。")

    def commodity_menu_cli(self):
        while True:
            print("\n--- 商品管理 ---")
            print("1. 添加商品类型")
            print("2. 设置出厂价格")
            print("3. 取消生产")
            print("4. 列出所有商品及生产情况")
            print("5. 删除商品类型")
            print("6. 返回上级菜单")
            sub = input("请选择：").strip()
            if sub == '1':
                name = input("商品名称：").strip()
                ok, msg = self.system.add_commodity(name)
                print(msg)
            elif sub == '2':
                code = input("国家代码：").strip()
                comm = input("商品名称：").strip()
                try:
                    price = float(input("出厂价格：").strip())
                    ok, msg = self.system.set_production_price(code, comm, price)
                    print(msg)
                except ValueError:
                    print("价格必须是数字。")
            elif sub == '3':
                code = input("国家代码：").strip()
                comm = input("商品名称：").strip()
                ok, msg = self.system.cancel_production(code, comm)
                print(msg)
            elif sub == '4':
                data = self.system.list_commodities()
                if not data:
                    print("暂无商品。")
                else:
                    for comm, prod_dict in data.items():
                        print(f"\n商品：{comm}")
                        if prod_dict:
                            print(f"{'国家代码':<6} {'出厂价格':<10}")
                            for code, price in prod_dict.items():
                                print(f"{code:<6} {price:<10.2f}")
                        else:
                            print("  无生产国")
            elif sub == '5':
                comm = input("商品名称：").strip()
                ok, msg = self.system.delete_commodity(comm)
                print(msg)
            elif sub == '6':
                break
            else:
                print("无效选项。")

    def tariff_menu_cli(self):
        while True:
            print("\n--- 关税管理 ---")
            print("1. 设置关税")
            print("2. 查看关税")
            print("3. 打印关税矩阵")
            print("4. 返回上级菜单")
            sub = input("请选择：").strip()
            if sub == '1':
                ex = input("出口国代码：").strip()
                im = input("进口国代码：").strip()
                comm = input("商品名称：").strip()
                try:
                    rate = float(input("税率(%): ").strip())
                    ok, msg = self.system.set_tariff(ex, im, comm, rate)
                    print(msg)
                except ValueError:
                    print("税率必须是数字。")
            elif sub == '2':
                ex = input("出口国代码：").strip()
                im = input("进口国代码：").strip()
                comm = input("商品名称：").strip()
                rate = self.system.get_tariff(ex, im, comm)
                print(f"{ex} -> {im}，{comm} 关税：{rate:.2f}%")
            elif sub == '3':
                comm = input("商品名称：").strip()
                codes, matrix = self.system.get_tariff_matrix(comm)
                if not codes:
                    print(f"商品 {comm} 不存在")
                    continue
                print(f"\n关税矩阵 - 商品：{comm}")
                header = "     " + "".join(f"{c:>8}" for c in codes)
                print(header)
                for i, ex in enumerate(codes):
                    row = f"{ex:<5}" + "".join(f"{matrix[i][j]:>8}" for j in range(len(codes)))
                    print(row)
            elif sub == '4':
                break
            else:
                print("无效选项。")

    def interactive_path_price_cli(self):
        comm = input("请输入商品名称：").strip()
        path_str = input("请输入路径（国家代码用 > 分隔，例如 CN>JP>US）：").strip()
        path = [p.strip() for p in path_str.split('>')]
        price, intermediates, err = self.system.compute_path_price(comm, path)
        if err:
            print(f"计算失败：{err}")
        else:
            print(f"\n计算结果：")
            for i, code in enumerate(path):
                print(f"到达 {code} 价格：{intermediates[i]:.2f}")
            print(f"最终价格：{price:.2f}")

    def interactive_best_path_cli(self):
        dest = input("请输入目的国代码：").strip()
        comm = input("请输入商品名称：").strip()
        hops_str = input("最大跳数（直接回车默认为4）：").strip()
        max_hops = int(hops_str) if hops_str else 4
        path, price, err = self.system.find_best_path(dest, comm, max_hops)
        if err:
            print(f"查询失败：{err}")
        else:
            print(f"最优路径：{' > '.join(path)}")
            print(f"最终价格：{price:.2f}")

    def manufacturing_reshoring_cli(self, home, comm):
        result, err, home_price = self.system.manufacturing_reshoring(home, comm)
        if err:
            print(f"分析失败：{err}")
            return

        print(f"\n本国: {home}  商品: {comm}  出厂价: {home_price:.2f}")
        print("关税调整建议（仅提高直接进口关税）:")

        changes = []  # 收集需要调整的 (出口国, 进口国, 商品, 新税率)
        for x_code, (new, old) in result.items():
            if isinstance(new, str):
                print(f"{x_code} -> {home}: {new} (当前 {old:.2f}%)")
            else:
                if new > old + 1e-9:
                    print(f"{x_code} -> {home}: 从 {old:.2f}% 提高到 {new:.2f}%")
                    changes.append((x_code, home, comm, new))
                else:
                    print(f"{x_code} -> {home}: 当前已足够 ({old:.2f}%)")

        if not changes:
            print("没有需要调整的关税。")
            return

        apply = input("\n是否应用上述关税调整？(y/n): ").strip().lower()
        if apply == 'y':
            try:
                self.system.begin_batch()
                for ex, im, cm, rate in changes:
                    self.system.set_tariff(ex, im, cm, rate)
                self.system.commit_batch()
                print(f"已应用 {len(changes)} 项关税调整。")
                print("提示：可使用「系统工具」中的「撤销最后一次关税修改」一键回滚本次所有调整。")
            except Exception as e:
                self.system.cancel_batch()
                print(f"应用调整时出错：{e}")
        else:
            print("未应用调整。")
            
    def system_tools_cli(self):
        while True:
            print("\n--- 系统工具 ---")
            print("1. 导出数据到文件")
            print("2. 从文件导入数据")
            print("3. 撤销最后一次关税修改")
            print("4. 返回上级菜单")
            sub = input("请选择：").strip()
            if sub == '1':
                path = input("请输入导出文件路径（如 data.json）：").strip()
                if not path:
                    print("路径不能为空。")
                    continue
                ok, msg = self.system.export_data(path)
                print(msg)
            elif sub == '2':
                path = input("请输入导入文件路径：").strip()
                if not path:
                    print("路径不能为空。")
                    continue
                confirm = input("导入将覆盖当前所有数据，确认吗？(y/n): ").strip().lower()
                if confirm == 'y':
                    ok, msg = self.system.import_data(path)
                    print(msg)
                else:
                    print("导入已取消。")
            elif sub == '3':
                ok, msg = self.system.undo_tariff_change()
                print(msg)
            elif sub == '4':
                break
            else:
                print("无效选项。")


# ==================== 主入口 ====================
def main():
    # 如果命令行参数带 --cli 则运行命令行版本，否则启动GUI
    if len(sys.argv) > 1 and sys.argv[1] == "--cli":
        cli = TariffCLI()
        cli.run()
    else:
        app = TariffGUI()
        app.mainloop()


if __name__ == "__main__":
    main()