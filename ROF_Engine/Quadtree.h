#ifndef __QUADTREE_H__
#define __QUADTREE_H__

#include "MathGeoLib/include/MathGeoLib.h"
#include <vector>

class GameObject;

struct QuadTreeNode
{
	QuadTreeNode* parent;
	QuadTreeNode* children[4];
	std::vector<GameObject*> bucket;
	AABB partition;

	QuadTreeNode();
	~QuadTreeNode();

	void AddGO(GameObject* go);

};

class QuadTreee
{
public:
	QuadTreee(AABB limits);
	~QuadTreee();

	void Clear();

	void DebugUpdate();

	void Insert(GameObject* go);
	void Remove(GameObject* go_to_delete);

public:
	QuadTreeNode* root = nullptr;
	AABB tree_limits;

};

#endif // !__QUADTREE_H__
