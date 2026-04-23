import json
import random

def generate_large_dataset(filename="tariff_data.json", num_countries=50, num_commodities=300):
    data = {
        "countries": {},
        "commodities": {},
        "production_prices": {},
        "tariffs": {}
    }

    # 1. Generate 50 Countries
    country_codes = []
    for i in range(1, num_countries + 1):
        code = f"c{i:02d}" # c01, c02...
        country_codes.append(code)
        data["countries"][code] = {
            "name": f"国家_{i}",
            "code": code,
            "transit_fee": round(random.uniform(5.0, 50.0), 2) # Transit fee between 5 and 50
        }

    # 2. Generate 300 Commodities
    commodity_names = []
    for i in range(1, num_commodities + 1):
        name = f"item_{i:03d}" # item_001, item_002...
        commodity_names.append(name)
        data["commodities"][name] = {"name": name}

    # 3. Generate Production Prices (Not every country makes every item to make it realistic)
    for comm in commodity_names:
        # Randomly select 5 to 15 countries that produce this commodity
        producers = random.sample(country_codes, random.randint(5, 15))
        for prod in producers:
            key = f"{prod}||{comm}"
            # Production price between 100 and 1000
            data["production_prices"][key] = round(random.uniform(100.0, 1000.0), 2)

    # 4. Generate Tariffs
    # To keep the file size manageable but robust, we'll generate tariffs only for 
    # a subset of trading pairs per commodity (e.g., 20% of possible routes).
    for comm in commodity_names:
        for ex in country_codes:
            for im in random.sample(country_codes, int(num_countries * 0.2)):
                if ex != im:
                    key = f"{ex}||{im}||{comm}"
                    # Tariffs between 0% and 40%
                    data["tariffs"][key] = round(random.uniform(0.0, 40.0), 2)

    with open(filename, 'w', encoding='utf-8') as f:
        json.dump(data, f, indent=2, ensure_ascii=False)
    
    print(f"Successfully generated {filename}!")
    print(f"Countries: {len(data['countries'])}")
    print(f"Commodities: {len(data['commodities'])}")
    print(f"Production Prices: {len(data['production_prices'])}")
    print(f"Tariffs: {len(data['tariffs'])}")

if __name__ == "__main__":
    generate_large_dataset()