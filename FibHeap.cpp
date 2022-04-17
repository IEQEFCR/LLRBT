#include<cstdio>

template <class T> class FibHeap {
	size_t _size;
	struct node {
	public:
		node* prev;
		node* next;
		node* child;
		node* parent;
		T value;
		int degree;
		bool marked;
		node (T value):value(value),prev(this),next(this),degree(0),marked(false),child(nullptr),parent(nullptr){}	
	};
	node* heap; //根链表里面value最小的堆节点
public:
	FibHeap() :heap(nullptr),_size(0){
	}
	
	~FibHeap() {
		if(heap) deleteAll(heap);
	}
	
	node* push(T value) {
		_size++;
		node* ret=new node(value);
		heap=_merge(heap,ret);//merge会返回较小的那一个
		return ret;
	}
	
	T pop() {
		_size--;
		node* old=heap;
		heap=_pop(heap);
		T ret=old->value;
		delete old;
		return ret;
	}
	
	void merge(FibHeap& other) { //可并操作
		heap=_merge(heap,other.heap);
		other.heap=nullptr;
	}
	
	bool empty() {
		return heap==nullptr;
	}
	
	T top() {
		return heap->value;
	}
	
	size_t size(){
		return _size;
	}

private:
	
	node* _merge(node* a,node* b) {
		if(a==nullptr)return b;
		if(b==nullptr)return a;
		if(a->value>b->value) {
			node* temp=a;   //交换，使a键值始终比b小
			a=b;
			b=temp;
		}
		node* anxt=a->next;node* bpre=b->prev;
		a->next=b,b->prev=a;
		anxt->prev=bpre,bpre->next=anxt;
		return a;
	}
	
	void deleteAll(node* n) {  //回收内存
		if(n!=nullptr) {
			node* tmp=n;
			do {
				node* d=tmp;
				tmp=tmp->next;
				deleteAll(tmp->child);
				delete d;
			} while(tmp!=n);
		}		
	}
	
	void addChild(node* parent,node* child) { //认儿子
		child->prev=child->next=child; //先把儿子前后剪掉（原来可能残留）
		child->parent=parent;
		parent->degree++;
		parent->child=_merge(parent->child,child); //再把儿子和兄弟连起来
	}
	
	void unlinkAll(node* n) { //把n和他兄弟全部把父节点解掉并且设置成儿子未删除
		if(n==nullptr)return;
		node* c=n;
		do {
			c->marked=false;
			c->parent=nullptr;
			c=c->next;
		}while(c!=n);
	}
	
	node* _pop(node* n) {
		unlinkAll(n->child);  //把n去掉，将其子节点拉到根链表上
		if(n->next==n) {  
			n=n->child;
		} else {
			n->next->prev=n->prev;
			n->prev->next=n->next;//先删n
			n=_merge(n->next,n->child);//再将所有子节点链接到根链表上
		}
		if(n==nullptr)return n;
		node* trees[64]={nullptr};  //开始合并度数相同的节点
		
		while(true) {
			if(trees[n->degree]!=nullptr) {
				node* t=trees[n->degree];
				if(t==n)break; //绕了一圈没有相同的，则结束
				trees[n->degree]=nullptr;
				if(n->value<t->value) {
					t->prev->next=t->next;
					t->next->prev=t->prev; //删除t，放入n的儿子
					addChild(n,t); 
				} else {//t的值要小一点，t当根
					t->prev->next=t->next;//先把t剪下来
					t->next->prev=t->prev; 
					if(n->next==n) { // 如果根链表只剩n，说明原来只有n和t两个，直接把单独的n放到单独的t儿子里
						t->next=t->prev=t;
						addChild(t,n);
						n=t; //现在指针指向根节点t
					} else { 
						n->prev->next=t; //将n换成t
						n->next->prev=t;
						t->next=n->next;
						t->prev=n->prev;
						addChild(t,n);
						n=t;
					}
				}
				continue;
			} else {
				trees[n->degree]=n;
			}
			n=n->next;
		}
		node* min=n;
		node* start=n;
		do {  //遍历根链表重新指定最小值
			if(n->value<min->value)min=n;
			n=n->next;
		} while(n!=start);
		return min;
	}
	
	node* _cut(node* heap,node* n) {  //把n剪下来和根链表链接
		if(n->next==n) { //独苗
			n->parent->child=nullptr;
		} else {
			n->next->prev=n->prev;
			n->prev->next=n->next;
			n->parent->child=n->next;
		}
		n->next=n->prev=n;
		n->marked=false;
		return _merge(heap,n);
	}

};

FibHeap<int> hp;

int main(){
	for(int i=1,tp;i<=5;i++){
		scanf("%d",&tp);
		hp.push(tp);
	}
	for(int i=1;i<=5;i++){
		printf("top:%d, size:%d\n",hp.top(),hp.size());
		hp.pop();
		printf("%d\n",hp.empty());
	}
	return 0;
}