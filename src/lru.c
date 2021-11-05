

#include <stdio.h>
#include <stdlib.h>

#define MEMBLOCK_QUANTITY 4
#define bool int
#define true 1
#define false 0

// ҳ����
int page_stream[] = {
		1, 2, 3, 5, 7, 3, 2, 3, 1, 5, 0, 8, 2, 1, 4,
};

typedef struct {
	int page;
	int access;
} Page;

// ����ҳ��
Page memory_table[MEMBLOCK_QUANTITY];

void print_memory();
void init_memory();
void request_page(int);

int main(void) {
	init_memory();
	print_memory("��ʼ��֮��");

	for (int i = 0, len = sizeof(page_stream)/sizeof(int); i < len; i ++) {
		request_page(page_stream[i]);
	}

	return EXIT_SUCCESS;
}

void init_memory()
{
	for (int i = 0; i < MEMBLOCK_QUANTITY; i ++) {
		Page *p = &memory_table[i];
		p->page = -1; // ��ʾ�˿����
	}

}

void print_memory(const char* title)
{
	puts(title);
	for (int i = 0; i < MEMBLOCK_QUANTITY; i ++) {
		Page *p = &memory_table[i];
		printf("\tblock=%d, page=%d, access=%d\n", i, p->page, p->access);
	}
}

/**
 * �������ڼ���ҳ���ҳ��
 * @return �Ƿ�����
 */
bool find_block(int page, int* block)
{
	// ���ҳ�������ڴ���, ֱ�ӷ���
	for (int i = 0; i < MEMBLOCK_QUANTITY; i ++) {
		Page *p = &memory_table[i];
		if (p->page == page) {
			printf("\tҳ��%d���ڿ�%d��, ����\n", page, i);
			*block = i;
			return true;
		}
	}

	int use_block = -1;
	int max_access = -1;
	for (int i = 0; i < MEMBLOCK_QUANTITY; i ++) {
		Page *p = &memory_table[i];
		// �����δʹ�õ�ҳ��, �򷵻�
		if (p->page == -1) {
			use_block = i;
			break;
		}
		else if (p->access > max_access) {
			max_access = p->access;
			use_block = i;
		}
	}
	*block = use_block;

	return false;
}

/**
 * ����ҳ��page��block
 */
void load_page(int page, int block)
{
	Page* p = &memory_table[block];
	p->page = page;
	p->access = 0;
}

void update_access(int page)
{
	for (int i = 0; i < MEMBLOCK_QUANTITY; i ++) {
		Page *p = &memory_table[i];
		if (page == p->page) {
			p->access = 0;
			continue;
		}
		p->access += 1;
	}
}
void request_page(int page)
{

	printf("����ҳ��%d\n", page);
	print_memory("  ����ҳ��֮ǰ");

	int block = -1;
	// ����
	if (find_block(page, &block)) {
		// ֻ���� ʱ��
		update_access(page);
	}
	else {
		load_page(page, block);
		update_access(page);
	}

	print_memory("  ����ҳ��֮��");
}
