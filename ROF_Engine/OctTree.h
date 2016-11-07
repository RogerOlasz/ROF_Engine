#ifndef __QUADTREE_H__
#define __QUADTREE_H__

#include "MathGeoLib/include/MathGeoLib.h"
#include <vector>

class GameObject;
class OctTree;

struct OctTreeNode
{
	OctTree* belonging = nullptr;

	OctTreeNode* parent = nullptr;
	OctTreeNode* children[8];
	std::vector<GameObject*> bucket;
	int max_bucket_capacity = 3;
	AABB partition;

	OctTreeNode(OctTree* my_tree);
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

	std::vector<OctTreeNode*> tree_struct;
};

#endif // !__QUADTREE_H__
