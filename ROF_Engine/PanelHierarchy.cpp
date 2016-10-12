#include "PanelHierarchy.h"
#include "Application.h"
#include <vector>

#include "ModuleGOManager.h"
#include "GameObject.h"

#include "ImGui/imgui.h"

PanelHierarchy::PanelHierarchy() : Panel("Hierarchy")
{

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

	//Code from ImGui Example
	//if (ImGui::TreeNode(App->go_manager->GetRootNode()->GetName()))
	//{
	//	ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());

	//	static int selection_mask = (1 << 2); // Dumb representation of what may be user-side selection state. You may carry selection state inside or outside your objects in whatever format you see fit.
	//	int node_clicked = -1;                // Temporary storage of what node we have clicked to process selection at the end of the loop. May be a pointer to your own node type, etc.
	//	ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, ImGui::GetFontSize() * 4); // Increase spacing to differentiate leaves from expanded contents.
	//	for (int i = 0; i < 6; i++)
	//	{
	//		// Disable the default open on single-click behavior and pass in Selected flag according to our selection state.
	//		ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ((selection_mask & (1 << i)) ? ImGuiTreeNodeFlags_Selected : 0);
	//		if (i < 3)
	//		{
	//			// Node
	//			bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, "Selectable Node %d", i);
	//			if (ImGui::IsItemClicked())
	//			{
	//				node_clicked = i;
	//			}
	//				
	//			if (node_open)
	//			{
	//				ImGui::Text("-----");
	//				ImGui::TreePop();
	//			}
	//		}
	//		else
	//		{
	//			// Leaf: The only reason we have a TreeNode at all is to allow selection of the leaf. Otherwise we can use BulletText() or TreeAdvanceToLabelPos()+Text().
	//			ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen, "Selectable Leaf %d", i);
	//			if (ImGui::IsItemClicked())
	//				node_clicked = i;
	//		}
	//	}
	//	if (node_clicked != -1)
	//	{
	//		// Update selection state. Process outside of tree loop to avoid visual inconsistencies during the clicking-frame.
	//		if (ImGui::GetIO().KeyCtrl)
	//			selection_mask ^= (1 << node_clicked);          // CTRL+click to toggle
	//		else //if (!(selection_mask & (1 << node_clicked))) // Depending on selection behavior you want, this commented bit preserve selection when clicking on item that is part of the selection
	//			selection_mask = (1 << node_clicked);           // Click to single-select
	//	}
	//	ImGui::PopStyleVar();
	//	ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());
	//	ImGui::TreePop();
	//}

	ImGui::End();
}

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