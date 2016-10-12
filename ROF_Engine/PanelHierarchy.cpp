#include "PanelHierarchy.h"
#include "Application.h"
#include <vector>
#include <string>

#include "ModuleGOManager.h"
#include "GameObject.h"

#include "ImGui/imgui.h"

PanelHierarchy::PanelHierarchy() : Panel("Hierarchy")
{
	active = true;
}

PanelHierarchy::~PanelHierarchy()
{

}

void PanelHierarchy::Draw()
{
	ImGui::Begin("Hierarchy", &active);
	
	//Lets start iterating my root children nodes to creat the tree hierarchy
	std::list<GameObject*>::iterator go_node = App->go_manager->GetRootNode()->children.begin();
	while (go_node != App->go_manager->GetRootNode()->children.end())
	{
		std::list<GameObject*>::iterator child_node = (*go_node)->children.begin();
		while (child_node != (*go_node)->children.end())
		{
			SceneTreeGameObject((*child_node));
			child_node++;
		}
		go_node++;
	}

	ImGui::End();
}

//Recursive method to create all game objects tree
void PanelHierarchy::SceneTreeGameObject(GameObject* node)
{
	if (node->HiddenFromHierarchy() == false)
	{
		//Disable the default open on single-click behavior and pass in Selected flag according to our selection state.
		ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

		if (selected_go == node)
		{
			node_flags += ImGuiTreeNodeFlags_Selected;
		}
		if (node->children.empty())
		{
			//Leaf: The only reason we have a TreeNode at all is to allow selection of the leaf. Otherwise we can use BulletText() or TreeAdvanceToLabelPos()+Text().
			node_flags += ImGuiTreeNodeFlags_Leaf;
		}

		if (ImGui::TreeNodeEx(node->GetName(), node_flags))
		{
			std::list<GameObject*>::iterator it = node->children.begin();
			while (it != node->children.end())
			{
				SceneTreeGameObject((*it));
				it++;
			}
		 ImGui::TreePop();
		}
	}
}