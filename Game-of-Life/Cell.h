#pragma once

class Cell
{
public:
	Cell() = default;
	~Cell() = default;

public:
	int get_neighbors();

public:
	bool is_live = false;
	bool next_live = false;

	Cell* neighbors[8];
};

