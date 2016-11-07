#include "OctTree.h"
#include "GameObject.h"
#include "DebugPainter.h"

OctTreeNode::OctTreeNode(OctTree* my_tree)
{
	belonging = my_tree;
	belonging->tree_struct.push_back(this);

	for (uint i = 0; i < 8; i++)
	{
		children[i] = nullptr;
	}
}

OctTreeNode::~OctTreeNode()
{

}

void OctTreeNode::AddGO(GameObject* go)
{
	if (bucket.size() < max_bucket_capacity && children[0] == nullptr)
	{
		bucket.push_back(go);
	}
	else
	{
		if (children[0] == nullptr)
		{
			vec half_box = ((partition.maxPoint - partition.minPoint) / 2.0f);
			
			//Creating childs [LOWER]
			children[0] = new OctTreeNode(this->belonging);
				
			children[0]->partition.minPoint.x = partition.minPoint.x;
			children[0]->partition.minPoint.y = partition.minPoint.y;
			children[0]->partition.minPoint.z = partition.minPoint.z;

			children[0]->partition.maxPoint.x = half_box.x;
			children[0]->partition.maxPoint.y = half_box.y;
			children[0]->partition.maxPoint.z = half_box.z;

			children[0]->parent = this;
				
			children[1] = new OctTreeNode(this->belonging);

			children[1]->partition.minPoint.x = half_box.x * 2;
			children[1]->partition.minPoint.y = partition.minPoint.y;
			children[1]->partition.minPoint.z = partition.minPoint.z;

			children[1]->partition.maxPoint.x = half_box.x;
			children[1]->partition.maxPoint.y = half_box.y;
			children[1]->partition.maxPoint.z = half_box.z;

			children[1]->parent = this;

			children[2] = new OctTreeNode(this->belonging);

			children[2]->partition.minPoint.x = half_box.x * 2;
			children[2]->partition.minPoint.y = partition.minPoint.y;
			children[2]->partition.minPoint.z = half_box.z * 2;

			children[2]->partition.maxPoint.x = half_box.x;
			children[2]->partition.maxPoint.y = half_box.y;
			children[2]->partition.maxPoint.z = half_box.z;

			children[2]->parent = this;

			children[3] = new OctTreeNode(this->belonging);

			children[3]->partition.minPoint.x = partition.minPoint.x;
			children[3]->partition.minPoint.y = partition.minPoint.y;
			children[3]->partition.minPoint.z = half_box.z * 2;

			children[3]->partition.maxPoint.x = half_box.x;
			children[3]->partition.maxPoint.y = half_box.y;
			children[3]->partition.maxPoint.z = half_box.z;

			children[3]->parent = this;

			//Creating childs [UPPER]
			children[4] = new OctTreeNode(this->belonging);

			children[4]->partition.minPoint.x = partition.minPoint.x;
			children[4]->partition.minPoint.y = half_box.y * 2;
			children[4]->partition.minPoint.z = partition.minPoint.z;

			children[4]->partition.maxPoint.x = half_box.x;
			children[4]->partition.maxPoint.y = half_box.y;
			children[4]->partition.maxPoint.z = half_box.z;

			children[4]->parent = this;

			children[5] = new OctTreeNode(this->belonging);

			children[5]->partition.minPoint.x = half_box.x * 2;
			children[5]->partition.minPoint.y = half_box.y * 2;
			children[5]->partition.minPoint.z = partition.minPoint.z;

			children[5]->partition.maxPoint.x = half_box.x;
			children[5]->partition.maxPoint.y = half_box.y;
			children[5]->partition.maxPoint.z = half_box.z;

			children[5]->parent = this;

			children[6] = new OctTreeNode(this->belonging);

			children[6]->partition.minPoint.x = half_box.x * 2;
			children[6]->partition.minPoint.y = half_box.y * 2;
			children[6]->partition.minPoint.z = half_box.z * 2;

			children[6]->partition.maxPoint.x = half_box.x;
			children[6]->partition.maxPoint.y = half_box.y;
			children[6]->partition.maxPoint.z = half_box.z;

			children[6]->parent = this;

			children[7] = new OctTreeNode(this->belonging);

			children[7]->partition.minPoint.x = partition.minPoint.x;
			children[7]->partition.minPoint.y = half_box.y * 2;
			children[7]->partition.minPoint.z = half_box.z * 2;

			children[7]->partition.maxPoint.x = half_box.x;
			children[7]->partition.maxPoint.y = half_box.y;
			children[7]->partition.maxPoint.z = half_box.z;

			children[7]->parent = this;

			for (uint i = 0; i < max_bucket_capacity - 1; i++)
			{
				const AABB* to_test = bucket.at(i)->GetBoundingBox();
				for (uint j = 0; j < 8; j++)
				{
					if (children[j]->partition.Intersects(*to_test))
					{
						children[j]->AddGO(bucket.at(i));
					}
				}
			}

			const AABB* to_test_actual = go->GetBoundingBox();
			for (uint i = 0; i < 8; i++)
			{
				if (children[i]->partition.Intersects(*to_test_actual))
				{
					children[i]->AddGO(go);
				}
			}

			bucket.clear();
		}
		else
		{
			const AABB* to_test_actual = go->GetBoundingBox();
			for (uint i = 0; i < 8; i++)
			{
				if (children[i]->partition.Intersects(*to_test_actual))
				{
					children[i]->AddGO(go);
				}
			}
		}		
	}
}

void OctTreeNode::DebugUpdate()
{
	if (bucket.size() == 0)
	{
		DebugDraw(partition, GreenYellow);
	}

	if (bucket.size() == 1)
	{
		DebugDraw(partition, Yellow);
	}

	if (bucket.size() == 2)
	{
		DebugDraw(partition, Orange);
	}	

	if (bucket.size() == 3)
	{
		DebugDraw(partition, Red);
	}
}

OctTree::OctTree(AABB limit)
{
	root = new OctTreeNode(this);
	root->parent = nullptr;
	root->partition = limit;
}

OctTree::~OctTree()
{

}

void OctTree::Insert(GameObject* go)
{
	root->AddGO(go);	
}