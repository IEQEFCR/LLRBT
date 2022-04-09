#include<cstdio>
#include <iostream>
template <typename T>
class Set {
		struct nd {
			T val;
			nd *ls{nullptr}, *rs{nullptr};
			size_t siz{0};
			bool color{0}; // 1 red, 0 black.
			nd(T val, bool color, size_t siz)
				: val(val), color(color), siz(siz) {}
			nd() {}
		} * Root{nullptr};

		size_t Siz(nd *now) {
			return now == nullptr ? 0 : now->siz;
		}
		bool col(nd *now) {
			return now == nullptr ? 0 : now->color;
		}
		nd *left_rotate(nd *now) {
			nd *tmp = now->rs;
			now->rs = tmp->ls;
			tmp->ls = now;
			tmp->color = now->color;
			now->color = 1;
			tmp->siz = Siz(now);
			now->siz = Siz(now->ls) + Siz(now->rs) + 1;
			return tmp;
		}
		nd *right_rotate(nd *now) {
			nd *tmp = now->ls;
			now->ls = tmp->rs;
			tmp->rs = now;
			tmp->color = now->color;
			now->color = 1;
			tmp->siz = Siz(now);
			now->siz = Siz(now->ls) + Siz(now->rs) + 1;
			return tmp;
		}
		void rev_col(nd *now) {
			now->color ^= 1;
			now->ls->color ^= 1;
			now->rs->color ^= 1;
		}

	public:
		bool exist(T key) {
			nd *x = Root;
			while (x != nullptr) {
				if (x->val == key)
					return (Siz(x) - Siz(x->ls) - Siz(x->rs));
				if (x->val > key)
					x = x->ls;
				else
					x = x->rs;
			}
			return 0;
		}

		nd *fix(nd *now) {
			if (col(now->rs) && !col(now->ls))
				now = left_rotate(now);
			if (col(now->ls) && col(now->ls->ls))
				now = right_rotate(now);
			if (col(now->ls) && col(now->rs))
				rev_col(now);
			now->siz = Siz(now->ls) + Siz(now->rs) + 1;
			return now;
		}

		nd *insert(nd *now, T key) {
			if (now == nullptr)
				return  new nd(key,1, 1);
			if (now->val < key)
				now->rs = insert(now->rs, key);
			else if (now->val > key)
				now->ls = insert(now->ls, key);
			return fix(now);
		}

		nd *deal(nd *now, bool opt) {
			if (opt) {
				rev_col(now);
				if (col(now->rs->ls)) {
					now->rs = right_rotate(now->rs);
					now = left_rotate(now);
					rev_col(now);
				}
				return now;
			} else {
				rev_col(now);
				if (col(now->ls->ls)) {
					now = right_rotate(now);
					rev_col(now);
				}
				return now;
			}
		}

		T getmin(nd *now) {
			nd *tmp = now;
			while (tmp->ls != nullptr)
				tmp = tmp->ls;
			return tmp->val;
		}

		nd *delete_min(nd *now) {
			if (now == nullptr) {
				delete now;
				return nullptr;
			}
			if (!col(now->ls) && !col(now->ls->ls))
				now = deal(now, 1);
			now->ls = delete_min(now->ls);
			return fix(now);
		}

		nd *earse(nd *now, T key) {
			if (key < now->val) {
				if (!col(now->ls) && !col(now->ls->ls))
					now = deal(now, 1);
				now = earse(now->ls, key);
			} else {
				if (col(now->ls))
					now = right_rotate(now);
				if (key == now->val && now->rs == nullptr) {
					delete now;
					return nullptr;
				}
				if (!col(now->rs) && !col(now->rs->ls))
					now = deal(now, 0);
				if (key == now->val) {
					now->val = getmin(now->rs);
					now->rs = delete_min(now->rs);
				} else
					now->rs = earse(now->rs, key);
			}
			return fix(now);
		}

		void insert(T key) {
			Root = insert(Root, key);
			Root->color = 0;
		}

		void earse(T key) {
			if (exist(key)) {
				if (!col(Root->ls) && !(col(Root->rs))) Root->color = 1;
				Root = earse(Root, key);
				if (Root != nullptr) Root->color = 0;
			}
		}
		void print_tree(nd *now) {
			if(now==nullptr) return ;
			if (now->ls) print_tree(now->ls);
			std::cout <<now->val<<" ";
			if (now->rs) print_tree(now->rs);
		}
		void print_tree() {
			print_tree(Root);
		}
};

Set<int> k;

int main() {
	for(int i=1;i<=6;i++){
		int tmp;
		scanf("%d",&tmp);
		k.insert(tmp);
	}
	k.print_tree();
}
