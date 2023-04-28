#include "Cell.h"

int Cell::get_neighbors()
{
    int cnt = 0;

	for (auto& cell : neighbors)
		if (cell->is_live)
			cnt++;

    return cnt;
}
