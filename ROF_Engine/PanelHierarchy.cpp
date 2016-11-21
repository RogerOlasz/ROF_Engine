#include "PanelHierarchy.h"
#include "Application.h"
#include "ModuleWindow.h"
#include <vector>

#include "ModuleGOManager.h"
#include "GameObject.h"

#include "ImGui/imgui.h"

PanelHierarchy::PanelHierarchy() : Panel("Hierarchy")
{
	active = true;
	set_size = true;
}

PanelHierarchy::~PanelHierarchy()
{

}

void PanelHierarchy::Draw()
{
	ImGui::SetNextWindowPos(ImVec2((App->window->GetWindowSize().x - (App->window->GetWindowSize().x - 4)), (App->window->GetWindowSize().y - (App->window->GetWindowSize().y - 25))));

	if (set_size == true)
	{
		ImGui::SetNextWindowSize(ImVec2(250, 610));
		set_size = false;
	}

	ImGui::Begin("Hierarchy", &active);
	
	//Start iterating my root children nodes to creat the tree hierarchy
	std::vector<GameObject*>::iterator go_node = App->go_manager->GetRootNode()->children.begin();
	while (go_node != App->go_manager->GetRootNode()->children.end())
	{
		GameObjectsHierarchy((*go_node));

		go_node++;
	}

	ImGui::End();
}

GameObject* PanelHierarchy::GetSelectedGO() const
{
	return selected_go;
}

void PanelHierarchy::SetSelectedGO(GameObject* go)
{
	selected_go = go;
}

//Recursive method to create all game objects tree
void PanelHierarchy::GameObjectsHierarchy(GameObject* node)
{
	//Disable the default open on single-click behavior and pass in Selected flag according to our selection state.
	ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

	if (node->children.empty())
	{
		//Leaf: The only reason we have a TreeNode at all is to allow selection of the leaf. Otherwise we can use BulletText() or TreeAdvanceToLabelPos()+Text().
		node_flags += ImGuiTreeNodeFlags_Leaf;
	}
	else
	{
		
	}

	if (selected_go == node)
	{
		node_flags += ImGuiTreeNodeFlags_Selected;
	}

	if (ImGui::TreeNodeEx(node->GetName(), node_flags))
	{
		std::vector<GameObject*>::iterator it = node->children.begin();
		while (it != node->children.end())
		{
			GameObjectsHierarchy((*it));
			it++;
		}
		ImGui::TreePop();
	}

	if (ImGui::IsItemClicked())
	{
		selected_go = node;
	}
}