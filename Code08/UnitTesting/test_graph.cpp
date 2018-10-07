#include "CppUnitTest.h"
#include "stdafx.h"
#include "graph.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace FrontendTests {
TEST_CLASS(TestGraph) {
  const string kProcName1 = "one";
  const string kProcName2 = "two";
  const string kProcName3 = "three";
  const string kProcName4 = "four";
  const string kProcName5 = "five";

 public:
  TEST_METHOD(TestAddNode) {
    Graph graph = Graph();
    graph.AddNode(kProcName1);
    graph.AddNode(kProcName1);

    // should only have 1 node if a duplicate node is added
    Assert::IsTrue(graph.GetSize() == 1);

    graph.AddNode(kProcName2);
    // two nodes now
    Assert::IsTrue(graph.GetSize() == 2);
  }

  TEST_METHOD(TestAddEdge) {
    Graph graph = Graph();
    // make sure graph is empty
    Assert::IsTrue(graph.GetSize() == 0);

    // graph size will be two if two edges that was not in the graph is added
    graph.AddEdge(kProcName1, kProcName2);
    graph.AddEdge(kProcName1, kProcName2);

    // even if the same edges are repeated it should not matter
    Assert::IsTrue(graph.GetSize() == 2);

    // kProcName1 should only have 1 neighbour kProcName2
    unordered_set<string> neighbours = graph.GetNeighbourNames(kProcName1);
    Assert::IsTrue(neighbours.size() == 1);
    Assert::IsTrue(neighbours.find(kProcName2) != neighbours.end());

    graph.AddEdge(kProcName1, kProcName3);
    Assert::IsTrue(graph.GetSize() == 3);

    // kProcName1 now has two neighbours
    neighbours = graph.GetNeighbourNames(kProcName1);
    Assert::IsTrue(neighbours.size() == 2);
    Assert::IsTrue(neighbours.find(kProcName3) != neighbours.end());
    Assert::IsTrue(graph.GetNeighbourNames(kProcName3).size() == 0);

    /* 1
      / \
     2<--3
    */
    graph.AddEdge(kProcName3, kProcName2);
    Assert::IsTrue(graph.GetSize() == 3);
    // kProcName1 still has 2 neighbours
    neighbours = graph.GetNeighbourNames(kProcName1);
    Assert::IsTrue(neighbours.size() == 2);
    // kProcName3 now has 1 neighbour
    Assert::IsTrue(graph.GetNeighbourNames(kProcName3).size() == 1);
  }

  // asserts that there is no cycle in the graph
  TEST_METHOD(TestToposort) {
    Graph graph = Graph();
    /*
      1 -> 2 -> 3 -> 5
           |    ^
           >--> 4
    */
    graph.AddEdge(kProcName1, kProcName2);
    graph.AddEdge(kProcName2, kProcName3);
    graph.AddEdge(kProcName2, kProcName4);
    graph.AddEdge(kProcName3, kProcName5);
    graph.AddEdge(kProcName4, kProcName3);

    Assert::IsTrue(graph.GetSize() == 5);

    vector<string> expected_result = vector<string>{
        kProcName5, kProcName3, kProcName4, kProcName2, kProcName1};
    vector<string> actual_result = graph.Toposort();
    Assert::IsTrue(expected_result == actual_result);


    // 1 -> 2 -> 3
    graph = Graph();
    graph.AddEdge(kProcName1, kProcName2);
    graph.AddEdge(kProcName2, kProcName3);

    expected_result = vector<string>{
        kProcName3, kProcName2, kProcName1};
    actual_result = graph.Toposort();
    Assert::IsTrue(expected_result == actual_result);
  }
};
}  // namespace FrontendTests