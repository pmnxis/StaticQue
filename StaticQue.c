#include <stdio.h>
#include <stdint.h>
 
typedef uint32_t key_t;
typedef uint32_t bool;

#define list_size 8
#define mem_noown 0x0
#define mem_using 0x1
#define failed (~0x0)
#ifndef nullptr
#define nullptr NULL
#endif // !nullptr
 

struct input_stamp {
	uint32_t lckList_idx;
	key_t keyval;
	uint32_t mcutime;
	uint32_t recount;
}typedef input_stamp;

struct StaticQue {
	bool lock;
	uint32_t cnt;
	/* list */
	input_stamp memStack[list_size];
	input_stamp *fifoArr[list_size];
	input_stamp *lckList[list_size];
}typedef StaticQue;

void SQ_init(StaticQue * target)
{
	// lock stuff
	for (uint32_t i = 0; i < list_size; i++)
	{
		target->memStack[i].lckList_idx = failed;
		target->memStack[i].keyval = NULL;
		target->memStack[i].mcutime = NULL;
		target->memStack[i].recount = NULL;
		target->fifoArr[i] = nullptr;
		target->lckList[i] = mem_noown;
	}
	target->cnt = 0;
}

input_stamp *SQ_getEmpty(StaticQue * target)
{
	if (target->cnt >= list_size)return failed;
	for (uint32_t i = 0; i<list_size; i++) if (target->lckList[i] == nullptr)return i;
	return failed;
}

void SQ_popque(StaticQue *target)
{
	// release
	target->lckList[target->fifoArr[0]->lckList_idx] = mem_noown;
	target->fifoArr[0]->lckList_idx = failed;	//  디버그용, 에러 유발
	target->fifoArr[0] = nullptr;	// 뒤에가면 이거 쓸필요가없음

	//  swap swap
	target->cnt--;

	for (uint32_t i = 0; i <= target->cnt; i++)
	{
		target->fifoArr[i] = target->fifoArr[i + 1];
		// whire로 바꾸고 i++혹은 ++i로 바꿀수잇으면 바꾸기
	}
	target->fifoArr[target->cnt] = nullptr;
}

void SQ_deque(StaticQue *target, uint32_t que_idx)
{
	// release
	target->lckList[target->fifoArr[que_idx]->lckList_idx] = mem_noown;
	target->fifoArr[que_idx]->lckList_idx = failed;	//  디버그용, 에러 유발
	target->fifoArr[que_idx] = nullptr;	// 뒤에가면 이거 쓸필요가없음

	//  swap swap
	target->cnt--;

	for (uint32_t i = que_idx; i <= target->cnt; i++)
	{
		target->fifoArr[i] = target->fifoArr[i + 1];
		// whire로 바꾸고 i++혹은 ++i로 바꿀수잇으면 바꾸기
	}
	target->fifoArr[target->cnt] = nullptr;
}

void SQ_push(StaticQue *target, uint32_t __keyval , uint32_t __timing)
{
	uint32_t new_idx = SQ_getEmpty(target);
	if (new_idx == failed)
	{
		SQ_popque(target);
		new_idx = SQ_getEmpty(target);
	}
	target->lckList[new_idx] = mem_using;
	target->fifoArr[target->cnt] = &(target->memStack[new_idx]);
	target->memStack[new_idx].lckList_idx = new_idx;
	target->memStack[new_idx].keyval = __keyval;
	target->memStack[new_idx].mcutime = __timing;
	target->cnt++;
}

int main()
{
	StaticQue Testment;
	SQ_init(&Testment);

	SQ_push(&Testment, 1, 100);
	SQ_push(&Testment, 2, 100);
	SQ_push(&Testment, 3, 100);
	SQ_push(&Testment, 4, 100);
	SQ_push(&Testment, 5, 100);
	SQ_push(&Testment, 6, 100);
	SQ_push(&Testment, 7, 100);
	SQ_push(&Testment, 8, 100);
	SQ_push(&Testment, 9, 100);
	SQ_push(&Testment, 10, 100);
	SQ_push(&Testment, 11, 100);
	SQ_push(&Testment, 12, 100);
	SQ_push(&Testment, 13, 100);
	SQ_push(&Testment, 14, 100);
	
	return 0;
}