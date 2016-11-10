#ifndef __OCTTREE_H__
#define __OCTTREE_H__

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
	template<class Type>
	void CollectCandidates(std::vector<GameObject*> &colliding, const Type &primitive) const;

};

class OctTree
{
public:
	OctTree(AABB limits);
	~OctTree();

	void Clear();

	void Insert(GameObject* go);
	template<class Type>
	void CollectCandidates(std::vector<GameObject*> &colliding, const Type &primitive) const;
	void AddCandidate(std::vector<GameObject*> &colliding, GameObject* candidate);

public:
	OctTreeNode* root = nullptr;

	std::vector<OctTreeNode*> tree_struct;
};

template<class Type>
inline void OctTree::CollectCandidates(std::vector<GameObject*> &colliding, const Type &primitive) const
{
	if (root != nullptr && primitive.Intersects(root->partition))
	{
		root->CollectCandidates(colliding, primitive); 
	}
}


template<class Type>
inline void OctTreeNode::CollectCandidates(std::vector<GameObject*> &colliding, const Type &primitive) const
{
	if (primitive.Intersects(partition))
	{
		//http://stackoverflow.com/questions/5346890/what-is-the-difference-between-const-iterator-and-iterator
		for (std::vector<GameObject*>::const_iterator tmp = bucket.begin(); tmp != bucket.end(); tmp++)
		{
			belonging->AddCandidate(colliding, (*tmp));
		}
	}

	for (uint i = 0; i < 8; i++)
	{
		if (children[i] != nullptr)
		{
			children[i]->CollectCandidates(colliding, primitive);
		}
	}
}

#endif // !__OCTTREE_H__
