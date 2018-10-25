#include "stdafx.h"
#include "CppUnitTest.h"
#include "graph.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace FrontendTests {
TEST_CLASS(TestGraph) {
  const Vertex kVertex1 = 0;
  const Vertex kVertex2 = 1;
  const Vertex kVertex3 = 2;
  const Vertex kVertex4 = 3;
  const Vertex kVertex5 = 4;
  const Vertex kVertex6 = 5;
  const Vertex kVertex7 = 6;

 public:
  TEST_METHOD(TestAddEdge) {
    Graph graph = Graph();
    // make sure graph is empty
    Assert::IsTrue(graph.GetSize() == 0);

    // graph size will be two if two edges that was not in the graph is added
    graph.AddEdge(kVertex1, kVertex2);
    graph.AddEdge(kVertex1, kVertex2);

    // even if the same edges are repeated it should not matter
    Assert::IsTrue(graph.GetSize() == 2);

    // kVertex1 should only have 1 neighbour kVertex2
    VertexSet neighbours = graph.GetNeighboursSet(kVertex1);
    Assert::IsTrue(neighbours.size() == 1);
    Assert::IsTrue(neighbours.find(kVertex2) != neighbours.end());

    graph.AddEdge(kVertex1, kVertex3);
    Assert::IsTrue(graph.GetSize() == 3);

    // kVertex1 now has two neighbours
    neighbours = graph.GetNeighboursSet(kVertex1);
    Assert::IsTrue(neighbours.size() == 2);
    Assert::IsTrue(neighbours.find(kVertex3) != neighbours.end());
    // kVertex3 has no outwards arrows
    Assert::IsTrue(graph.GetNeighboursSet(kVertex3).size() == 0);

    /* 1
      / \
     2<--3
    */
    graph.AddEdge(kVertex3, kVertex2);
    Assert::IsTrue(graph.GetSize() == 3);
    // kVertex1 still has 2 neighbours
    neighbours = graph.GetNeighboursSet(kVertex1);
    Assert::IsTrue(neighbours.size() == 2);
    // kVertex3 now has 1 neighbour
    Assert::IsTrue(graph.GetNeighboursSet(kVertex3).size() == 1);
  }

  // asserts that there is no cycle in the graph
  TEST_METHOD(TestToposort) {
    Graph graph = Graph();
    /*
      1 -> 2 -> 3 -> 5
           |    ^
           >--> 4
    */
    graph.AddEdge(kVertex1, kVertex2);
    graph.AddEdge(kVertex2, kVertex3);
    graph.AddEdge(kVertex2, kVertex4);
    graph.AddEdge(kVertex3, kVertex5);
    graph.AddEdge(kVertex4, kVertex3);

    Assert::IsTrue(graph.GetSize() == 5);

    vector<Vertex> expected_result = vector<Vertex>{
        kVertex5, kVertex3, kVertex4, kVertex2, kVertex1};
    vector<Vertex> actual_result = graph.Toposort();
    Assert::IsTrue(expected_result == actual_result);

    // Add a disconnected component
    /*
      1 -> 2 -> 3 -> 5     6 -> 7
           |    ^
           >--> 4
    */
    graph.AddEdge(kVertex6, kVertex7);

    // additional 7->6 at the end
    expected_result =
        vector<Vertex>{kVertex5, kVertex3, kVertex4, kVertex2,
                       kVertex1, kVertex7, kVertex6};
    actual_result = graph.Toposort();
    Assert::IsTrue(expected_result == actual_result);
  }
};
}  // namespace FrontendTests