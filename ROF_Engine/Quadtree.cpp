#include "Quadtree.h"
#include "GameObject.h"
#include "DebugPainter.h"

QuadTreeNode::QuadTreeNode()
{

}

QuadTreeNode::~QuadTreeNode()
{

}

void QuadTreeNode::AddGO(GameObject* go)
{
	if (bucket.size() <= 2 && children[0] == nullptr)
	{
		bucket.push_back(go);
	}
	else
	{
		if (children[0] == nullptr)
		{
			vec half_min_point = (this->partition.minPoint / 2);
			vec half_max_point = (this->partition.maxPoint / 2);

			AABB box1;
			AABB box2;
			AABB box3;
			AABB box4;

			box1.Enclose(half_min_point, half_max_point);

		}
		
	}
}

QuadTreee::QuadTreee(AABB limits)
{
	root = new QuadTreeNode();
	root->parent = nullptr;

	tree_limits = limits;
}

QuadTreee::~QuadTreee()
{

}

void QuadTreee::DebugUpdate()
{
	DebugDraw(tree_limits, Green);
}

void QuadTreee::Insert(GameObject* go)
{
	root->AddGO(go);
	/*QuadTreeNode* new_go_node = new QuadTreeNode();

	if (root->children.size() < 4)
	{
		new_go_node->parent = root;
		root->children.push_back(new_go_node);
	}*/
	
}