#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <set>
#include <map>


struct Point
{
	int id;
	double x;
	double y;
	int color = 0;

	Point(int id, double x, double y) : id(id), x(x), y(y) {}

};



struct Edge
{
	Point* start;
	Point* finish;
	double len;

	Edge(Point* p1, Point* p2) : start(p1), finish(p2)
	{
		len = pow((pow(p1->x - p2->x, 2) + pow(p1->y - p2->y, 2)), 0.5);
	}

	bool operator<(const Edge& obj) const
	{
		return len < obj.len;
	}
};


class Graph
{
public:

	std::vector<Point*> points_list;
	std::set<Edge> edges_list;
	std::vector<Edge> minimal_tree;

	Graph() = default;

	void recolor(int color1, int color2)
	{
		for (Point* point : points_list)
		{
			if (point->color == color1)
			{
				point->color = color2;
			}
			if (point->color == color1)
			{
				point->color = color2;
			}
		}

	}

	bool create_minimal_tree()
	{
		int last_color = 1;
		int num_colors = 1;

		//std::sort(edges_list.begin(), edges_list.end());

		for (Edge new_Edge : edges_list)
		{
			if (new_Edge.finish->color == new_Edge.start->color && new_Edge.finish->color != 0) {}

			else if (new_Edge.start->color != 0 && new_Edge.finish->color != 0 && new_Edge.start->color != new_Edge.finish->color)
			{
				recolor(new_Edge.start->color, new_Edge.finish->color);
				num_colors -= 1;
				minimal_tree.push_back(new_Edge);
			}

			else if (new_Edge.start->color != 0)
			{
				new_Edge.finish->color = new_Edge.start->color;
				minimal_tree.push_back(new_Edge);
			}

			else if (new_Edge.finish->color != 0)
			{
				new_Edge.start->color = new_Edge.finish->color;
				minimal_tree.push_back(new_Edge);
			}

			else if (new_Edge.finish->color == 0 && new_Edge.start->color == 0)
			{
				new_Edge.finish->color = last_color;
				new_Edge.start->color = last_color;
				last_color += 1;
				num_colors += 1;
				minimal_tree.push_back(new_Edge);
			}

			if (num_colors == 1)
			{
				return true;
			}
		}

		return false;
	}

};

class Parser
{
public:
	std::string filename;
	std::vector<Point*> points_list;
	std::set<Edge> edges_list;
	Parser(const std::string& filename) : filename(filename) {}
	void parse()
	{
		std::ifstream inFile(filename);

		if (!inFile)
		{
			std::cerr << "Unable to open file " << filename << std::endl;
			return;
		}

		double first, second;
		int id;

		while (inFile >> id >> first >> second)
		{
			Point* point = new Point(id, first, second);
			points_list.push_back(point);
		}

		inFile.close();

		for (size_t i = 0; i < points_list.size(); ++i)
		{
			for (size_t j = i + 1; j < points_list.size(); ++j)
			{
				if (points_list[i]->id != points_list[j]->id)
				{
					edges_list.insert(Edge(points_list[i], points_list[j]));
				}
			}
		}

		/*for (size_t i = 0; i < edges_list.size(); ++i)
		{
			std::cout << edges_list[i].start->id << ", " << edges_list[i].finish->id << ", " << edges_list[i].len << std::endl;
		}*/

		return;

	};
};

std::ostream& operator<<(std::ostream& os, const Graph& Graph) {
	for (int i = 0; i < Graph.minimal_tree.size(); i++)
	{
		os << " " << Graph.minimal_tree[i].start->id << " <-----> " << Graph.minimal_tree[i].finish->id << " " << std::endl;
	};
	return os;
};

int main()
{
	//Graph myGraph = Graph();
	std::string x;
	std::cin >> x;
	//std::vector<Point> all_p;
	//std::vector<Edge> all_w;

	Graph myGraph = Graph();

	const std::string filename = "data.txt";

	Parser parser = Parser(filename);
	parser.parse();

	myGraph.points_list = parser.points_list;
	myGraph.edges_list = parser.edges_list;

	myGraph.create_minimal_tree();

	std::cout << myGraph;

	std::cout << (myGraph.create_minimal_tree() == true) << std::endl;

}