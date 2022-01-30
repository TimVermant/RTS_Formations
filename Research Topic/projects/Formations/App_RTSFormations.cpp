//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

using namespace Elite;

#include "App_RTSFormations.h"

App_RTSFormations::~App_RTSFormations()
{
	SAFE_DELETE(m_pFormation);
}

void App_RTSFormations::Start()
{
	//Initialization of your application. If you want access to the physics world you will need to store it yourself.
	//----------- CAMERA ------------
	DEBUGRENDERER2D->GetActiveCamera()->SetZoom(50.f);
	DEBUGRENDERER2D->GetActiveCamera()->SetCenter(Elite::Vector2(70, 50));
	DEBUGRENDERER2D->GetActiveCamera()->SetMoveLocked(false);


	m_pFormation = new Formation{ m_NrOfUnits };

	AddUnits();
}

void App_RTSFormations::Update(float deltaTime)
{

	UpdateUI();

	m_pFormation->Update(deltaTime);
}

void App_RTSFormations::UpdateUI()
{
	//Setup
	int menuWidth = 200;
	int const width = DEBUGRENDERER2D->GetActiveCamera()->GetWidth();
	int const height = DEBUGRENDERER2D->GetActiveCamera()->GetHeight();
	bool windowActive = true;


	ImGui::SetNextWindowPos(ImVec2((float)width - menuWidth - 10, 10));
	ImGui::SetNextWindowSize(ImVec2((float)menuWidth, (float)height - 90));
	ImGui::Begin("Gameplay Programming", &windowActive, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
	ImGui::PushAllowKeyboardFocus(false);
	ImGui::SetWindowFocus();
	ImGui::PushItemWidth(70);
	//Elements
	ImGui::Text("CONTROLS");
	ImGui::Indent();
	ImGui::Unindent();

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();
	ImGui::Spacing();

	ImGui::Text("STATS");
	ImGui::Indent();
	ImGui::Text("%.3f ms/frame", 1000.0f / ImGui::GetIO().Framerate);
	ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
	ImGui::Unindent();

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();
	ImGui::Spacing();

	ImGui::Text("Battle formations");
	ImGui::Spacing();
	ImGui::Spacing();

	if (ImGui::Button("Make formation"))
	{
		std::cout << "Button pressed" << std::endl;
		m_pFormation->CreateFormation(m_TrimWorldSize);
	}

	
	ImGui::InputInt("Units Per Line", &m_pFormation->GetUnitsPerLine());

	ImGui::InputFloat("Distance between units on line", &m_pFormation->GetDistanceBetweenUnitsOnLine());
	ImGui::InputFloat("Vertical distance between units on line", &m_pFormation->GetVerticalDistanceBetweenUnitsOnLine());
	ImGui::InputFloat("Distance between lines", &m_pFormation->GetDistanceBetweenLines());

	//End
	ImGui::PopAllowKeyboardFocus();
	ImGui::End();
}

void App_RTSFormations::Render(float deltaTime) const
{
	//Draw boundaries
	std::vector<Elite::Vector2> points =
	{
		{ -m_TrimWorldSize, m_TrimWorldSize },
		{ m_TrimWorldSize, m_TrimWorldSize },
		{ m_TrimWorldSize, -m_TrimWorldSize },
		{ -m_TrimWorldSize, -m_TrimWorldSize }
	};
	DEBUGRENDERER2D->DrawPolygon(&points[0], 4, { 1,0,0,1 }, 0.4f);

	m_pFormation->Render(deltaTime);
}

void App_RTSFormations::AddUnits()
{


	for (size_t i = 0; i < m_NrOfUnits; i++)
	{
		Vector2 pos = Elite::randomVector2(-m_TrimWorldSize, m_TrimWorldSize);
		m_pFormation->AddUnit(new BattleUnitAgent{ pos, Elite::Color{0.66f,0.42f,0.31f,1.f} });
	}
}
