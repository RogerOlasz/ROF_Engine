#ifndef __QUADTREE_H__
#define __QUADTREE_H__

#include "MathGeoLib/include/MathGeoLib.h"
#include <vector>

class GameObject;

template<class Type>
class QuadTreeNode
{
public:
	QuadTreeNode<Type>*			parent;
	QuadTreeNode<Type>*			children[4];
	std::vector<Type*> bucket;

public:

	QuadTreeNode();

	~QuadTreeNode();

	void Insert();
	void CollectIntersections(std::vector<GameObject*> &objects, const Type &primitive) const;

};

template<class Type>
class QuadTree
{
public:
	QuadTree();
	~QuadTree();

	void Create(AABB limits);
	void Clear();

	void Insert(GameObject* go);
	void Remove(GameObject* go_to_delete);

	bool Intersect(std::vector<GameObject*> &objects, Type &primitive);

public:
	QuadTreeNode<Type>* root;

};

#endif // !__QUADTREE_H__
