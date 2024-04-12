#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <map>


struct Point
{
	int id;
	double x;
	double y;
	int color = 0;

	Point(int id, double x, double y) : id(id), x(x), y(y) {}

};



struct Way
{
	Point* start;
	Point* finish;
	double len;

	Way(Point* p1, Point* p2) : start(p1), finish(p2)
	{
		len = pow((pow(p1->x - p2->x, 2) + pow(p1->y - p2->y, 2)), 0.5);
	}

	bool operator<(const Way& obj) const
	{
		return len < obj.len;
	}
};


class Graph
{
public:

	std::vector<Point*> points_list;
	std::vector<Way> ways_list;
	std::vector<Way> minimal_tree;

	Graph() = default;

	Graph(std::string filename)
	{
		std::ifstream inFile(filename);

		if (!inFile) 
		{
			std::cerr << "Unable to open file data.txt" << std::endl;
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

		//for (size_t i = 0; i < points_list.size(); ++i)
		//{
		//	std::cout << points_list[i].id << ", " << points_list[i].x << ", " << points_list[i].y << std::endl;
		//}

		for (size_t i = 0; i < points_list.size(); ++i)
		{
			for (size_t j = i + 1; j < points_list.size(); ++j)
			{
				if (points_list[i]->id != points_list[j]->id)
				{
					ways_list.push_back(Way(points_list[i], points_list[j]));
				}
			}
		}

		for (size_t i = 0; i < ways_list.size(); ++i)
		{
			std::cout << ways_list[i].start->id << ", " << ways_list[i].finish->id << ", " << ways_list[i].len << std::endl;
		}

		return;

	};

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

		std::sort(ways_list.begin(), ways_list.end());

		for (Way new_way : ways_list)
		{
			if (new_way.finish->color == new_way.start->color && new_way.finish->color != 0) {}

			else if (new_way.start->color != 0 && new_way.finish->color != 0 && new_way.start->color != new_way.finish->color)
			{
				recolor(new_way.start->color, new_way.finish->color);
				num_colors -= 1;
				minimal_tree.push_back(new_way);
			}

			else if (new_way.start->color != 0)
			{
				new_way.finish->color = new_way.start->color;
				minimal_tree.push_back(new_way);
			}

			else if (new_way.finish->color != 0)
			{
				new_way.start->color = new_way.finish->color;
				minimal_tree.push_back(new_way);
			}

			else if (new_way.finish->color == 0 && new_way.start->color == 0)
			{
				new_way.finish->color = last_color;
				new_way.start->color = last_color;
				last_color += 1;
				num_colors += 1;
				minimal_tree.push_back(new_way);
			}

			if (num_colors == 1) 
			{
				return true;
			}
		}

		return false;
	}

};

std::ostream& operator<<(std::ostream& os, const Graph& Graph) {
	for (int i = 0; i < Graph.minimal_tree.size(); i++) 
	{
		os << " " << Graph.minimal_tree[i].start->id << " <-----> " << Graph.minimal_tree[i].finish->id << " " << std::endl;
	};
	return os;
};

/*std::vector < std::pair<double, double> > Parsing()
{
	std::ifstream inFile("data.txt");
	if (!inFile) {
		std::cerr << "Unable to open file data.txt" << std::endl;
		return {};
	}
	std::vector < std::pair<double, double> > data;
	double first, second;
	int nul;
	while (inFile >> nul >> first >> second) {
		data.push_back(std::make_pair(first, second));
	}

	inFile.close();
	for (const auto& pair : data) {
		std::cout << pair.first << ", " << pair.second << std::endl;
	}

	return data;


};*/



int main()
{
	//Graph myGraph = Graph();
	std::string x;
	std::cin >> x;
	//std::vector<Point> all_p;
	//std::vector<Way> all_w;

	Graph myGraph = Graph(x);

	myGraph.create_minimal_tree();

	std::cout << myGraph;

	std::cout << (myGraph.create_minimal_tree() == true) << std::endl;

}