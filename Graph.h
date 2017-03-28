/*
Graph.h
图算法，使用邻接链表表示
2016/10/23
*/

#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
struct Vertex
{
	int index;   // 索引号
	int distance;  // 距离
	Vertex *pre;   // 前节点
	bool known;    // 是否已知
};
#endif