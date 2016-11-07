#include "OctTree.h"
#include "GameObject.h"
#include "DebugPainter.h"

OctTreeNode::OctTreeNode()
{

}

OctTreeNode::~OctTreeNode()
{

}

void OctTreeNode::AddGO(GameObject* go)
{
	if (bucket.size() <= 2 && children[0] == nullptr)
	{
		bucket.push_back(go);
	}
	else
	{
		if (children[0] == nullptr)
		{
			vec half_box = ((partition.maxPoint - partition.minPoint) / 2.0f);

			for (uint i = 0; i < 8; i++)
			{
				children[i] = new OctTreeNode();
				//children[i]->partition.Enclose();
			}
		}
		
	}
}

void OctTreeNode::DebugUpdate()
{
	DebugDraw(partition, Green);
}

OctTree::OctTree(AABB limit)
{
	root = new OctTreeNode();
	root->parent = nullptr;
	root->partition = limit;
}

OctTree::~OctTree()
{

}

void OctTree::Insert(GameObject* go)
{
	root->AddGO(go);
	/*QuadTreeNode* new_go_node = new QuadTreeNode();

	if (root->children.size() < 4)
	{
		new_go_node->parent = root;
		root->children.push_back(new_go_node);
	}*/
	
}