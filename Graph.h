/*
Graph.h
ͼ�㷨��ʹ���ڽ������ʾ
2016/10/23
*/

#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
struct Vertex
{
	int index;   // ������
	int distance;  // ����
	Vertex *pre;   // ǰ�ڵ�
	bool known;    // �Ƿ���֪
};
#endif