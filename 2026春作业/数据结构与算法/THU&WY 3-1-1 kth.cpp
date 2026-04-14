#include <cstdio>

struct nd {
    int val;
    int cnt = 1;
    nd* p = nullptr;
    nd* lc = nullptr;
    nd* rc = nullptr;
    int ht = 0;
    int sz = 1;
    nd(int v, nd* p) : val(v), p(p) {}
};

nd* root = nullptr;

int h(nd* a) { return a ? a->ht : -1; }
int s(nd* a) { return a ? a->sz : 0; }

void us(nd* c) {
    if (c) c->sz = c->cnt + s(c->lc) + s(c->rc);
}

bool uh(nd* c) {
    if (!c) return true;
    int o = c->ht;
    c->ht = (h(c->lc) > h(c->rc) ? h(c->lc) : h(c->rc)) + 1;
    return o == c->ht;
}

void connect(nd* c, nd* p, bool isL) {
    if (p) {
        if (isL) p->lc = c;
        else p->rc = c;
    }
    if (c) c->p = p;
}

void cn342(nd* a, nd* t1, nd* b, nd* t2, nd* c) {
    connect(t1, a, false); connect(t2, c, true);
    connect(a, b, true); connect(c, b, false);
    uh(a); us(a); uh(c); us(c);
    uh(b); us(b);
}

void ua(nd* g) {
    while (g) {
        us(g);
        uh(g);
        nd* gp = g->p;
        nd* l = g->lc; nd* r = g->rc;
        bool isR = (g == root);
        bool isL = (gp && g == gp->lc);

        if (h(l) > h(r) + 1) {
            if (h(l->lc) >= h(l->rc)) {
                connect(l, gp, isL); if(isR) root = l;
                cn342(l->lc, l->lc ? l->lc->rc : nullptr, l, l->rc, g);
            } else {
                nd* v = l->rc;
                connect(v, gp, isL); if(isR) root = v;
                cn342(l, v->lc, v, v->rc, g);
            }
        } else if (h(r) > h(l) + 1) {
            if (h(r->rc) >= h(r->lc)) {
                connect(r, gp, isL); if(isR) root = r;
                cn342(g, r->lc, r, r->rc ? r->rc->lc : nullptr, r->rc);
            } else {
                nd* v = r->lc;
                connect(v, gp, isL); if(isR) root = v;
                cn342(g, v->lc, v, v->rc, r);
            }
        }
        g = g->p;
    }
}

nd* loc(int x) {
    nd* tmp = root;
    if (!tmp) return nullptr;
    while (true) {
        if (x > tmp->val && tmp->rc) tmp = tmp->rc;
        else if (x < tmp->val && tmp->lc) tmp = tmp->lc;
        else return tmp;
    }
}

nd* suc(nd* c) {
    c = c->rc;
    while (c && c->lc) c = c->lc;
    return c;
}

void ins(int x) {
    if (!root) { root = new nd(x, nullptr); return; }
    nd* c = loc(x);
    if (x == c->val) {
        c->cnt++;
        nd* t = c; while(t) { us(t); t = t->p; }
    } else if (x > c->val) { c->rc = new nd(x, c); ua(c); }
    else { c->lc = new nd(x, c); ua(c); }
}

void rem(int x) {
    nd* c = loc(x);
    if (!c || c->val != x) return;
    if (--(c->cnt) > 0) {
        nd* t = c; while(t) { us(t); t = t->p; }
        return;
    }
    if (c->lc && c->rc) {
        nd* d = suc(c);
        c->val = d->val; c->cnt = d->cnt;
        c = d; // Deleting successor instead
    }
    nd* p = c->p;
    nd* v = c->lc ? c->lc : c->rc;
    if (p) {
        if (c == p->lc) connect(v, p, true);
        else connect(v, p, false);
        ua(p);
    } else {
        root = v;
        if (v) v->p = nullptr;
    }
    delete c;
}

int fin(int x) {
    nd* tmp = root;
    while (tmp) {
        int rs = s(tmp->rc);
        if (x <= rs) tmp = tmp->rc;
        else if (x > rs + tmp->cnt) {
            x -= (rs + tmp->cnt);
            tmp = tmp->lc;
        } else return tmp->val;
    }
    return -1;
}

int main() {
    int n, a, b;
    if (scanf("%d", &n) != 1) return 0;
    while (n--) {
        scanf("%d%d", &a, &b);
        if (a == 1) ins(b);
        else if (a == 2) rem(b);
        else printf("%d\n", fin(b));
    }


    return 0;
}