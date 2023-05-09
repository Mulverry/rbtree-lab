// _RBTREE_H_라는 상수가 정의되어 있는지 확인하고, 선언되지 않았을 때만 아래 헤더를 컴파일한다. 가장 흔한 헤더 중복 사용 방지 방법이라고 함.
#ifndef _RBTREE_H_
#define _RBTREE_H_

// 왜 넣은건지 모르겠는 헤더. 이 코드에서 사용 안하는듯?
#include <stddef.h>

// 익명 열거형을 선언하고, color_t라는 별칭 부여
typedef enum { RBTREE_RED, RBTREE_BLACK } color_t;

// int 타입에 key_t라는 별칭 부여(변수 타입 관리를 위함인듯)
typedef int key_t;

// node_t 구조체 선언부. 멤버로 color_t(열거형) color; key_t(int) key; struct node_t(구조체) *parent, *left, *right; 들을 갖는다. 또한, 동일한 이름의 별칭 부여
typedef struct node_t {
  color_t color;
  key_t key;
  struct node_t *parent, *left, *right;
} node_t;

// rbtree 메인 구조체 선언부
typedef struct {
  node_t *root;
  node_t *nil;  // for sentinel
} rbtree;

rbtree *new_rbtree(void);
void delete_rbtree(rbtree *);

node_t *rbtree_insert(rbtree *, const key_t);

node_t *rbtree_find(const rbtree *, const key_t);
node_t *rbtree_min(const rbtree *);
node_t *rbtree_max(const rbtree *);
int rbtree_erase(rbtree *, node_t *);

int rbtree_to_array(const rbtree *, key_t *, const size_t);

#endif  // _RBTREE_H_
