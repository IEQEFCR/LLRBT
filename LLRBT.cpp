#include<cstdio>
#include <iostream>
template <typename T> 
class Set {
	struct nd {
		T val;
		nd *ls;
		nd *rs;
		size_t siz;
		bool color; // 1 red, 0 black.
		nd(T val, bool color=0, size_t siz=1)
		: val(val), color(color), siz(siz),ls(nullptr),rs(nullptr) {}
	}*Root;
	
	
	void deleteAll(nd *now){ //回收内存
		if(now!=nullptr){
			deleteAll(now->ls);
			deleteAll(now->rs);
			now->ls=now->rs=nullptr;
			delete now;
		}
	}
	
	size_t _size(nd *now) {
		return now == nullptr ? 0 : now->siz;
	}
	
	bool _color(nd *now) {
		return now == nullptr ? 0 : now->color;
	}
	
	nd *left_rotate(nd *now) {
		nd *tmp = now->rs;
		now->rs = tmp->ls;
		tmp->ls = now;
		tmp->color = now->color;
		now->color = 1;
		tmp->siz = _size(now);
		now->siz = _size(now->ls) + _size(now->rs) + 1;
		return tmp;
	}
	
	nd *right_rotate(nd *now) {
		nd *tmp = now->ls;
		now->ls = tmp->rs;
		tmp->rs = now;
		tmp->color = now->color;
		now->color = 1;
		tmp->siz = _size(now);
		now->siz = _size(now->ls) + _size(now->rs) + 1;
		return tmp;
	}
	
	void rev_col(nd *now) {
		now->color ^= 1;
		now->ls->color ^= 1;
		now->rs->color ^= 1;
	}
	
	nd *fix(nd *now) {
		if (_color(now->rs) && !_color(now->ls))
			now = left_rotate(now);
		if (_color(now->ls) && _color(now->ls->ls))
			now = right_rotate(now);
		if (_color(now->ls) && _color(now->rs))
			rev_col(now);
		now->siz = _size(now->ls) + _size(now->rs) + 1;
		return now;
	}
	
	nd *deal(nd *now, bool opt) {
		rev_col(now);
		if (opt) {
			if (_color(now->rs->ls)) {
				now->rs = right_rotate(now->rs);
				now = left_rotate(now);
				rev_col(now);
			}
		} else {
			if (_color(now->ls->ls)) {
				now = right_rotate(now);
				rev_col(now);
			}
		}
		return now;
	}
	
	T getmin(nd *now) {
		nd *tmp = now;
		while (tmp->ls != nullptr)
			tmp = tmp->ls;
		return tmp->val;
	}
	
	nd *delete_min(nd *now) {
		if (now->ls == nullptr) {
			delete now;
			return nullptr;
		}
		if (!_color(now->ls) && !_color(now->ls->ls))
			now = deal(now, 1);
		now->ls = delete_min(now->ls);
		return fix(now);
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
	
	nd *earse(nd *now, T key) {
		if (key < now->val) {
			if (!_color(now->ls) && !_color(now->ls->ls))
				now = deal(now, 1);
			now->ls = earse(now->ls, key);
		} else {
			if (_color(now->ls))
				now = right_rotate(now);
			if (key == now->val && now->rs == nullptr) {
				delete now;
				return nullptr;
			}
			if (!_color(now->rs) && !_color(now->rs->ls))
				now = deal(now, 0);
			if (key == now->val) {
				now->val = getmin(now->rs);
				now->rs = delete_min(now->rs);
			} else
				now->rs = earse(now->rs, key);
		}
		return fix(now);
	}
	
public:
	Set(){
		Root =nullptr;
	}
	
	~Set(){
		deleteAll(Root);
	}
	
	bool empty(){
		return _size(Root)==0;
	}
	
	size_t count(T key) { //查找是否存在key ,由于LLRBT元素不可重，所以只会返回0或者1
		nd * now = Root;
		while (now != nullptr) {
			if (now->val == key) return 1;
			if (now->val > key) now = now->ls;
			else now = now->rs;
		}
		return 0;
	}

	void clear(){
		deleteAll(Root);
	}
	
	size_t size(){
		return _size(Root);
	}
	
	void insert(T key) {
		Root = insert(Root, key);
		Root->color = 0;
	}
	
	void earse(T key) {
		if (count(key)) {
			if (!_color(Root->ls) && !(_color(Root->rs))) Root->color = 1;
			Root = earse(Root, key);
			if (Root != nullptr) Root->color = 0;
		}
	}
	
	void print(nd *now) {
		if(now==nullptr) return ;
		if (now->ls) print(now->ls);
		std::cout <<now->val<<" ";
		if (now->rs) print(now->rs);
	}
	void print() {
		print(Root);
	}
};

Set<int> k;

int main() {
	for(int i=1;i<=6;i++){
		int tmp;
		scanf("%d",&tmp);
		k.insert(tmp);
	}
	for(int i=1;i<=6;i++){
		int tmp;
		scanf("%d",&tmp);
		k.earse(tmp);
		k.print();
	}
	return 0;
}
