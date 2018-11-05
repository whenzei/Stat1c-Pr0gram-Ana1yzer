#include "stdafx.h"
#include "CppUnitTest.h"
#include "graph.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace FrontendTests {
TEST_CLASS(TestGraph) {
  const Vertex kVertex1 = 1;
  const Vertex kVertex2 = 2;
  const Vertex kVertex3 = 3;
  const Vertex kVertex4 = 4;
  const Vertex kVertex5 = 5;
  const Vertex kVertex6 = 6;
  const Vertex kVertex7 = 7;

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

    VertexList expected_result =
        VertexList{kVertex5, kVertex3, kVertex4, kVertex2, kVertex1};
    VertexList actual_result = graph.Toposort();
    Assert::IsTrue(expected_result == actual_result);

    // Add a disconnected component
    /*
      1 -> 2 -> 3 -> 5     6 -> 7
           |    ^
           >--> 4
    */
    graph.AddEdge(kVertex6, kVertex7);

    // additional 7->6 at the end
    expected_result = VertexList{kVertex5, kVertex3, kVertex4, kVertex2,
                                 kVertex1, kVertex7, kVertex6};
    actual_result = graph.Toposort();
    Assert::IsTrue(expected_result == actual_result);
  }

  TEST_METHOD(TestIsEmpty) {
    Graph graph = Graph();
    Assert::IsTrue(graph.IsEmpty());

    graph.AddEdge(kVertex1, kVertex2);
    Assert::IsFalse(graph.IsEmpty());
  }

  TEST_METHOD(TestHasCycle) {
    Graph graph = Graph();

    graph.AddEdge(kVertex1, kVertex2);
    graph.AddEdge(kVertex2, kVertex3);

    Assert::IsFalse(graph.HasCycle());

    // test disjointed components
    graph.AddEdge(kVertex4, kVertex5);
    Assert::IsFalse(graph.HasCycle());

    // should now have cycle
    graph.AddEdge(kVertex3, kVertex4);
    graph.AddEdge(kVertex5, kVertex1);
    Assert::IsTrue(graph.HasCycle());
  }

  TEST_METHOD(TestGetNeighbours) {
    Graph graph = Graph();

    graph.AddEdge(kVertex1, kVertex2);
    graph.AddEdge(kVertex1, kVertex3);
    graph.AddEdge(kVertex1, kVertex5);
    // duplicate edges should not appear in both set and list
    graph.AddEdge(kVertex1, kVertex2);

    VertexSet expected_set = VertexSet{kVertex2, kVertex3, kVertex5};
    VertexList expected_list = VertexList{kVertex2, kVertex3, kVertex5};

    Assert::IsTrue(expected_list == graph.GetNeighboursList(kVertex1));
    Assert::IsTrue(expected_set == graph.GetNeighboursSet(kVertex1));

    // graph is directed
    Assert::IsTrue(VertexSet() == graph.GetNeighboursSet(kVertex2));
  }

  TEST_METHOD(TestGetUnreachableVertices) {
    Graph graph = Graph();

    graph.AddEdge(kVertex1, kVertex2);
    // kVertex2 branches out to 3 and 4
    graph.AddEdge(kVertex2, kVertex3);
    graph.AddEdge(kVertex2, kVertex4);
    // 3 and 4 coalesce to 5
    graph.AddEdge(kVertex3, kVertex5);
    graph.AddEdge(kVertex4, kVertex5);

    // all vertices should be unreachable if kVertex1 is removed (since it is
    // root too)
    Assert::IsTrue(
        VertexSet{kVertex1, kVertex2, kVertex3, kVertex4, kVertex5} ==
        graph.GetUnreachableVertices(kVertex1));

    // same for kVertex2
    Assert::IsTrue(VertexSet{kVertex2, kVertex3, kVertex4, kVertex5} ==
                   graph.GetUnreachableVertices(kVertex2));

    // kVertex3 will only make itself unreachable since there exists a path to
    // all other vertex through 4
    Assert::IsTrue(VertexSet{kVertex3} ==
                   graph.GetUnreachableVertices(kVertex3));

    // adding alternative path means 2 doesnt block as many
    graph.AddEdge(kVertex1, kVertex4);
    Assert::IsTrue(VertexSet{kVertex2, kVertex3} ==
                   graph.GetUnreachableVertices(kVertex2));
  }

  TEST_METHOD(TestCanReach) {
    Graph graph = Graph();
    graph.AddEdge(kVertex1, kVertex2);
    graph.AddEdge(kVertex1, kVertex3);
    graph.AddEdge(kVertex2, kVertex4);
    graph.AddEdge(kVertex2, kVertex5);
    graph.AddEdge(kVertex3, kVertex5);
    graph.AddEdge(kVertex5, kVertex3);
    graph.AddEdge(kVertex6, kVertex3);

    int result1 = graph.CanReach(kVertex1, kVertex3);
    Assert::IsTrue(result1);
    int result2 = graph.CanReach(kVertex2, kVertex3);
    Assert::IsTrue(result2); 
    int result3 = graph.CanReach(kVertex3, kVertex5);
    Assert::IsTrue(result3);
    int result4 = graph.CanReach(kVertex5, kVertex3);
    Assert::IsTrue(result4);
    int result5 = graph.CanReach(kVertex5, kVertex6);
    Assert::IsFalse(result5);

    graph.RemoveEdge(kVertex2, kVertex5);
    int result6 = graph.CanReach(kVertex2, kVertex3);
    Assert::IsFalse(result6);
  }
};
}  // namespace FrontendTests