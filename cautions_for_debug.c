// rbtree.c 파일에서


#include "rbtree.h" 
#include <stdio.h>
#include <stdlib.h>
//<rbtree.h>가 아니라 "rbtree.h"로 변경.

rbtree *new_rbtree(void)
{
	rbtree *t = (rbtree *)calloc(1, sizeof(rbtree));
	t->nil = (node_t *)calloc(1, sizeof(node_t)); 
	t->nil->color = RBTREE_BLACK;
	t->root = t->nil;
	return t;
}

	// node_t *nil = (node_t *)calloc(1, sizeof(node_t)); 를 했더니 노드 안에 노드를 설정하는 꼴이 되버려서 segment 에러가 계속 발생.
  // t->nil = (node_t *)calloc(1, sizeof(node_t)); 로 수정
  
void insert_fixup(rbtree *t, node_t *node_inserted)
{
	node_t *grandparent = node_inserted->parent->parent;
	node_t *parent = node_inserted->parent;
	// 삽입 노드 부모 색이 BLACK -> 문제 X
	if (parent->color == RBTREE_BLACK)
	{
		return;
	}
	// 삽입 노드 부모 색이 RED -> 문제 발생
	else
	{
		// 삼촌 노드 찾기
		node_t *node_uncle;
		if (grandparent->left == parent)
		{
			node_uncle = grandparent->right;
		}
		else
		{
			node_uncle = grandparent->left;
		}
		// Case 1
		if (node_uncle->color == RBTREE_RED)
		{
			parent->color = RBTREE_BLACK;
			node_uncle->color = RBTREE_BLACK;
			grandparent->color = RBTREE_RED;
			if (t->root == grandparent)
			{
				t->root->color = RBTREE_BLACK;
			}
			insert_fixup(t, grandparent);
		}
		// Case 2, 3
		else
		{
			// Case 2-1 (오른쪽으로 꺾임)
			if (parent->left == node_inserted && grandparent->right == parent)
			{
				// TODO: 부모 기준으로 오른쪽으로 회전 - Case 3로 만들어줌
				right_rotate(t, parent);
				parent = node_inserted;
			}
			// Case 2-2 (왼쪽으로 꺾임)
			else if (parent->right == node_inserted && grandparent->left == parent)
			{
				// TODO: 부모 기준으로 왼쪽으로 회전 - Case 3 로 만들어줌
				left_rotate(t, parent);
				parent = node_inserted;
			}
			// Case 3
			color_t temp_color = parent->color;
			parent->color = grandparent->color;
			grandparent->color = temp_color;
			if (grandparent->left == parent)
			{
				right_rotate(t, grandparent);
			}
			else
			{
				left_rotate(t, grandparent);
			}
		}
	}
}

// 교과서 sudo code랑 어설프게 합쳐보겠다고 while문 썼다가 망함.
// while (node_to_insert->parent->color == RBTREE_RED) {} 가 반복되면서 트리삽입을 다 꼬아놓음.


//Makefile에서
//DSENTINEL 주석 해제.
.PHONY: test clean

CFLAGS=-I ../common -Wall -O -DSENTINEL

test: test-rbtree
	./test-rbtree
	valgrind -s --leak-check=full ./test-rbtree

clean:
	rm -f test-rbtree *.o

test-rbtree: test-rbtree.o rbtree.o

test-rbtree.o: ../common/test-rbtree.c
	$(CC) $(CFLAGS) -c ../common/test-rbtree.c



//test-rbtree.c파일에서
//곳곳에 printf함수로 진행과정을 넣어주며 어디서 버그가 발생하는지를 추적함.
//보다시피 test_find_erase_fixed는 test_find_erase-> rbtree_insert -> insert_fixup 등으로 유기적으로 연관되어있음.
//test_find_erase_fixed에서 버그가 발생하면 erase함수가 문제가 아니라 insert함수 문제일 수도 있음.
//때문에 어디서 버그가 발생하는지 추적할 필요성이 있음.

void test_find_erase(rbtree *t, const key_t *arr, const size_t n) {
  printf("5-2-1통과\n");
  for (int i = 0; i < n; i++) {
    node_t *p = rbtree_insert(t, arr[i]);
    assert(p != NULL);
  }
  printf("5-2-2통과\n");
  for (int i = 0; i < n; i++) {
    node_t *p = rbtree_find(t, arr[i]);
    assert(p != NULL);
    assert(p->key == arr[i]);
    rbtree_erase(t, p);
  }
  printf("5-2-3통과\n");
  for (int i = 0; i < n; i++) {
    node_t *p = rbtree_find(t, arr[i]);
    assert(p == NULL);
  }
  printf("5-2-4통과\n");
  for (int i = 0; i < n; i++) {
    node_t *p = rbtree_insert(t, arr[i]);
    assert(p != NULL);
    node_t *q = rbtree_find(t, arr[i]);
    assert(q != NULL);
    assert(q->key == arr[i]);
    assert(p == q);
    rbtree_erase(t, p);
    q = rbtree_find(t, arr[i]);
    assert(q == NULL);
  }
  // printf("Debug\n");
}

void test_find_erase_fixed() {
  const key_t arr[] = {10, 5, 8, 34, 67, 23, 156, 24, 2, 12, 24, 36, 990, 25};
  const size_t n = sizeof(arr) / sizeof(arr[0]);
  rbtree *t = new_rbtree();
  assert(t != NULL);
  printf("5-1통과\n");
  test_find_erase(t, arr, n);
  printf("5-2통과\n");
  delete_rbtree(t);
  printf("5-3통과\n");
}
  
