//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

//Includes
#include "App_GraphTheory.h"
#include "framework\EliteAI\EliteGraphs\EliteGraphAlgorithms\EEularianPath.h"

using namespace Elite;

//Destructor
App_GraphTheory::~App_GraphTheory()
{
	SAFE_DELETE(m_pGraph2D);
}

//Functions
void App_GraphTheory::Start()
{
	//Initialization of your application. If you want access to the physics world you will need to store it yourself.
	//----------- CAMERA ------------
	DEBUGRENDERER2D->GetActiveCamera()->SetZoom(80.f);
	DEBUGRENDERER2D->GetActiveCamera()->SetCenter(Elite::Vector2(0, 0));
	DEBUGRENDERER2D->GetActiveCamera()->SetMoveLocked(false);
	DEBUGRENDERER2D->GetActiveCamera()->SetZoomLocked(false);


	//Create Graph
	m_pGraph2D = new Graph2D<GraphNode2D, GraphConnection2D>(false);

	//Add nodes
	m_pGraph2D->AddNode(new GraphNode2D(0, Vector2(10.f, 30.f)));
	m_pGraph2D->AddNode(new GraphNode2D(1, Vector2(20.f, -10.f)));
	m_pGraph2D->AddNode(new GraphNode2D(2, Vector2(30.f, -10.f)));
	m_pGraph2D->AddNode(new GraphNode2D(3, Vector2(-30.f, 20.f)));
	m_pGraph2D->AddNode(new GraphNode2D(4, Vector2(-40.f, 20.f)));


	//Add connection
	m_pGraph2D->AddConnection(new GraphConnection2D(0, 1));
	m_pGraph2D->AddConnection(new GraphConnection2D(1, 2));
	m_pGraph2D->AddConnection(new GraphConnection2D(3, 0));
	m_pGraph2D->AddConnection(new GraphConnection2D(2, 0));
	m_pGraph2D->AddConnection(new GraphConnection2D(4, 0));
	m_pGraph2D->AddConnection(new GraphConnection2D(4, 3));


}

void App_GraphTheory::Update(float deltaTime)
{

	m_GraphEditor.UpdateGraph(m_pGraph2D);
	m_pGraph2D->SetConnectionCostsToDistance();

	auto eulerFinder = EulerianPath<GraphNode2D, GraphConnection2D>(m_pGraph2D);
	auto eulerianity = eulerFinder.IsEulerian();

	/*switch (eulerianity)
	{
	case Elite::Eulerianity::notEulerian:
		std::cout << "Not Eulerian\n";
		break;
	case Elite::Eulerianity::semiEulerian:
		std::cout << "Semi Eulerian\n";
		break;
	case Elite::Eulerianity::eulerian:
		std::cout << "Eulerian\n";
		break;
	default:
		std::cout << "Default?\n";
		break;
	}*/

	//------- UI --------
#ifdef PLATFORM_WINDOWS
#pragma region UI
	{
		//Setup
		int menuWidth = 150;
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

		ImGui::Text("Graph Theory");
		ImGui::Spacing();
		ImGui::Spacing();
		if (ImGui::Button("Check Euler"))
		{
			auto path = eulerFinder.FindPath(eulerianity);
		}

		//End
		ImGui::PopAllowKeyboardFocus();
		ImGui::End();
	}
#pragma endregion
#endif


}

void App_GraphTheory::Render(float deltaTime) const
{
	m_GraphRenderer.RenderGraph(m_pGraph2D, true, true, true, true);
}
