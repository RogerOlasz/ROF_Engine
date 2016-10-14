#include "PanelHierarchy.h"
#include "Application.h"
#include "ModuleWindow.h"
#include <vector>
#include <string>

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
		SceneTreeGameObject((*go_node));

		go_node++;
	}

	ImGui::End();
}

GameObject* PanelHierarchy::GetSelectedGO() const
{
	return selected_go;
}

//Recursive method to create all game objects tree
void PanelHierarchy::SceneTreeGameObject(GameObject* node)
{
	if (node->GetHierarchyState() == true)
	{
		//Disable the default open on single-click behavior and pass in Selected flag according to our selection state.
		ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

		if (node->children.empty())
		{
			//Leaf: The only reason we have a TreeNode at all is to allow selection of the leaf. Otherwise we can use BulletText() or TreeAdvanceToLabelPos()+Text().
			node_flags += ImGuiTreeNodeFlags_Leaf;
		}

		if (ImGui::TreeNodeEx(node->GetName(), node_flags))
		{
			std::vector<GameObject*>::iterator it = node->children.begin();
			while (it != node->children.end())
			{
				SceneTreeGameObject((*it));
				it++;
			}
		 ImGui::TreePop();
		}

		if (ImGui::IsItemClicked())
		{
			node_flags += ImGuiTreeNodeFlags_Selected;
			selected_go = node;
		}
	}
}