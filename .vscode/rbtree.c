#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>

rbtree* new_rbtree(void) {
	rbtree* p = (rbtree*)calloc(1, sizeof(rbtree));
	// TODO: initialize struct if needed
	p->nil = (node_t*)calloc(1, sizeof(node_t));
	p->root = p->nil;
	p->nil->color = RBTREE_BLACK;
	return p;
}

void delete_rbtree(rbtree* t) {
	// TODO: reclaim the tree nodes's memory: RBtree 구조체가 차지했던 메모리 반환
	delete_one(t, t->root);
	free(t->nil);
	free(t);
}

void delete_one(rbtree* t, node_t* cur) {
	if (cur != t->nil) {
		delete_one(t, cur->left);
		delete_one(t, cur->right);
		free(cur);
	}
}

void rotate_left(rbtree *t, node_t *x){
	node_t *y = x->right;
	x->right = y->left; //y의 왼쪽의 서버트리를 x의 오른쪽 서버트리로 옮김
	if (y->left != t->nil){
		y->left->parent = x;
	}
	y -> parent = x ->parent;
	if (x-> parent == t->nil){ //x가 루트노드라면
		t->root = y;
	}
	else if (x == x->parent->left){ //x가 왼쪽 자식일 경우
		x->parent->left = y;
	}
	else {
		x->parent->right = y;
	}
	y->left = x;
	x->parent = y;
}

void rotate_right(rbtree *t, node_t *x){
	node_t *y = x->left;
	x->left = y->right; //y의 오른쪽 서버트리를 x의 왼쪽 서버트리로 옮김
	if (y->right != t->nil){
		y->right->parent = x;
	}
	y -> parent = x ->parent;
	if (x-> parent == t->nil){ //x가 루트노드라면
		t->root = y;
	}
	else if (x == x->parent->right){
		x->parent->right = y;
	}
	else {
		x->parent->left = y;
	}
	y->right = x;
	x->parent = y;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  node_t *node_to_insert = (node_t *)calloc(1, sizeof(node_t));
  node_to_insert->key = key;
  
  // 어디에다 노드를 집어넣을 건지 확인
  node_t *x = t->root;
  node_t *y = t->nil;
  while (x != t->nil){ //루트노드가 존재하는 동안
    y = x;
    if (node_to_insert-> key < x->key){ //새로 집어넣는 노드의 key가 루트노드의 key보다 작으면
      x = x->left;
    } else {
		x= x->right;
	}
  }
  node_to_insert->parent = y;
  if (y == t->nil){ 	//node_to_insert의 부모 노드가 없을 때, node_to_insert는 루트노드가 됨.
	t->root = node_to_insert;
  }
  else if (node_to_insert->key < y->key){//node_to_insert의 key가 부모노드의 것보다 작으면 왼쪽 자식이 됨.
	y->left = node_to_insert;
  }
  else{y->right = node_to_insert;}
  node_to_insert->left = t->nil;
  node_to_insert->right = t->nil;
  node_to_insert->color = RBTREE_RED;
  rbtree_insert_fixup(t, node_to_insert);
  return node_to_insert;
}

node_t* rbtree_insert_fixup(rbtree* t, node_t *node_to_insert){
	node_t *grandparent = node_to_insert->parent->parent;
	while (node_to_insert->parent->color == RBTREE_RED){
		//case1. 부모, 삼촌 ==RED -> 부모삼촌 모두 black으로 바꾸고, 할아버지는 red.
		if (node_to_insert->parent == grandparent->left){
			node_t *uncle = grandparent->right;
			if (uncle->color == RBTREE_RED){
				node_to_insert->parent->color == RBTREE_BLACK;
				uncle->color == RBTREE_BLACK;
				grandparent->color == RBTREE_RED;
				node_to_insert = grandparent; // while문으로 다시 할아버지 확인
			} else { // case2, case3. 부모는 red인데 삼촌이 black인 경우
				//case2.
				if (node_to_insert == node_to_insert->parent->right){
					node_to_insert = node_to_insert->parent; 
					rotate_left(t, node_to_insert); //부모를 기준으로 왼쪽으로 회전
				}
				grandparent->color = RBTREE_RED;
				node_to_insert -> parent -> color = RBTREE_BLACK;
				rotate_right(t, grandparent);
			}
		}else{
			node_t *uncle = grandparent->left;
			if (uncle->color == RBTREE_RED){
				node_to_insert->parent->color = RBTREE_BLACK;
				uncle->color = RBTREE_BLACK;
				grandparent->color = RBTREE_RED;
				node_to_insert = grandparent;
			} else {
				if (node_to_insert == node_to_insert->parent->left){
					node_to_insert = node_to_insert->parent;
					rotate_left(t, node_to_insert);
				}
				grandparent->color = RBTREE_RED;
				node_to_insert->parent->color = RBTREE_BLACK;
				rotate_left(t, grandparent);
			}
		}
	}
	t->root->color = RBTREE_BLACK;
	return t->root;
}

node_t* rbtree_find(const rbtree* t, const key_t key) {
	// TODO: implement find
	//  RB tree내에 해당 key가 있는지 탐색하여 있으면 해당 노드 반환.
	node_t *tmp = t->root;
	if (tmp != t->nil){
		while (tmp -> key != key){
			if (tmp->key > key){ //tmp가 가리키는 key값이 찾아야 할 key값보다 크면, tmp를 줄여서 key값에 가깝게해야 함.
				tmp = tmp->left;
			} else {
				tmp = tmp->right;
			}
		}
		return tmp; //tmp->key == key라면 바로 tmp 반환
	} return NULL; // root노드가 nil이라면 NULL 반환
}

node_t* rbtree_min(const rbtree* t) {
	// TODO: implement find: tree의 min key를 가지는 노드반환
	node_t* tmp = t->root;
	if (tmp != NULL) {
		while (tmp->left != NULL) {
			tmp = tmp->left;
		}
	}
	return tmp;
}

node_t* rbtree_max(const rbtree* t) {
	// TODO: implement find: tree의 max key를 가지는 노드반환
	node_t* tmp = t->root;
	if (tmp != NULL) {
		while (tmp->right != NULL) {
			tmp = tmp->right;
		}
	}
	return tmp;
}

int rbtree_erase(rbtree* t, node_t* p) {
	// TODO: implement erase: 특정 노드 삭제 작업
	return 0;
}

int rbtree_to_array(const rbtree* t, key_t* arr, const size_t n) {
	// TODO: implement to_array
	return 0;
}