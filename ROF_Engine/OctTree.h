#ifndef __QUADTREE_H__
#define __QUADTREE_H__

#include "MathGeoLib/include/MathGeoLib.h"
#include <vector>

class GameObject;

struct OctTreeNode
{
	OctTreeNode* parent;
	OctTreeNode* children[8];
	std::vector<GameObject*> bucket;
	AABB partition;

	OctTreeNode();
	~OctTreeNode();

	void DebugUpdate();

	void AddGO(GameObject* go);

};

class OctTree
{
public:
	OctTree(AABB limits);
	~OctTree();

	void Clear();

	void Insert(GameObject* go);
	void Remove(GameObject* go_to_delete);

public:
	OctTreeNode* root = nullptr;

};

#endif // !__QUADTREE_H__
