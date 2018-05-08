#include "timberwolf.h"

extern std::vector <Pin*> PinVector;
extern std::vector <Component*> ComponentVector;
extern std::unordered_map < std::string, int > ComponentHash, PinHash;
int num_rows, num_cols, x_min, x_max, y_min, y_max, w, h, core_width, core_height, component_width;
std::vector<bool> bool_grid;
std::vector<int> area_grid;
float *d_x, *d_y, *x, *y;
Matrix <float> *L_BiCG;

void read_design (char * FileName)
{
	std::size_t last;
	std::ifstream iFile;
	std::string token, name;
	std::stringstream iLine;
	Point <float> location;
	float x, y, core_width_f, core_height_f;
	int i, ComponentID=1, PinID=1;

	// Check if you can open the input file
	iFile.open(FileName);
	if (!iFile.is_open())
	{
		std::cout << "\033[1;31mERROR: cannot open the file\033[0m" << FileName << "for reading\n";
		return;
	}
	iFile >> token; str_toupper(token);
	do {
		switch(token[0])
		{
			case('I'): 
				iFile >> name >> token;
				str_toupper(name);
				str_toupper(token);
				switch(token[0])
				{
					case('L'): 
						iFile >> x >> y;
						location = Point<float>(x, y);
						PinVector.push_back(new Pin(name, location));
						PinHash[name] = PinID++;
						break;
					case('C'):
						getline(iFile, token);
						last = token.find_last_not_of(' ');
						token = token.substr(0, last+1);
						iLine.str(token); iLine.clear();
						int i = PinHash[name] - 1;
						while(!iLine.eof())
						{
							iLine >> token;
							str_toupper(token);
							PinVector[i]->add_connection(token);
						}
						break;
				}
				break;
			case('C'):
				iFile >> name >> token;
				str_toupper(name);
				ComponentVector.push_back(new Component(name));
				ComponentHash[name] = i = ComponentID++; i--;
				ComponentVector[i]->set_location(Point<float>(0.0f, 0.0f));
				getline(iFile, token);
				last = token.find_last_not_of(' ');
				token = token.substr(0, last+1);
				iLine.str(token); iLine.clear();
				while(!iLine.eof())
				{
					iLine >> token;
					str_toupper(token);
					ComponentVector[i]->add_connection(token);
				}
				break;
			case('W'):
				iFile >> token >> core_width_f >> token >> core_height_f;
			default: getline(iFile, token);
		}
		iFile >> token;
		str_toupper(token)
	} while(!iFile.eof());
	iFile.close();
	
	core_width  = core_width_f  * 1000.0;
	core_height = core_height_f * 1000.0;

	num_rows = core_height / CORE_Y_OFF;
	num_cols = core_width / CORE_X_OFF;
	component_width = COMPONENT_WIDTH / CORE_X_OFF;
	std::cout << "Core Height: " << core_height_f << std::endl;
	std::cout << "Core Width: " << core_width_f << std::endl;
	std::cout << "# of ROWS: " << num_rows << std::endl;
	std::cout << "# of COLS: " << num_cols << std::endl;
	std::cout << "# of COLS / Component: " << component_width << std::endl;
}

void minimise_QPWL()
{
	int i, j;
	int ComponentSize = ComponentVector.size() + 1;
	int PinSize = PinVector.size() + 1;
	float val;

	Matrix <float> L ("Laplacian Matrix", ComponentSize, ComponentSize);
	L_BiCG = new Matrix <float> ("Laplacian Matrix", ComponentSize, ComponentSize);
	Matrix <float> P ("Pin Connection Matrix", ComponentSize, PinSize);

	d_x = new float[ComponentSize]();
	d_y = new float[ComponentSize]();
	x   = new float[ComponentSize]();
	y   = new float[ComponentSize]();

	for (auto& it: PinVector)
	{
		j = PinHash[it->name()];
		val = (float) (2.0f / ((float)it->connection().size() + 1.0f));
		for(auto& a: it->connection())
		{
			i = ComponentHash[a];
			P(i,j) += val;
		}
	}

	for (auto& it: ComponentVector)
	{
		i = ComponentHash[it->name()];
		val = (float) (2.0f / ((float)it->connection().size() + 1.0f));
		for(auto& a: it->connection())
		{
			j = ComponentHash[a];
			// Adjacency Matrix
			L(i, j) -= val;
			L(j, i) -= val;
			// Degree Matrix
			L(i, i) += val;
			L(j, j) += val;
		}

		d_x[i] = 0.0;
		d_y[i] = 0.0;
		for (auto& a: PinVector)
		{
			j = PinHash[a->name()];
			d_x[i] += (P(i, j) * a->location().x());
			d_y[i] += (P(i, j) * a->location().y());
			L(i, i) += P(i, j);
			L(j, j) += P(i, j);
		}
	}
	for (int i = 0; i < ComponentSize; ++i)
		for (int j = 0; j < ComponentSize; ++j)
			(*L_BiCG)(i,j) = L(i,j);

	L.LU();
	L.solve(x, d_x);
	L.solve(y, d_y);
	int k=1;
	for (auto& it: ComponentVector)
	{
		it->set_location(Point<float>(x[k], y[k]));
		k++;
	}
}

void minimise_density()
{
	int i, j, x_i, y_i, direction;
	float x_f, y_f;
	int rows = (num_rows+2);
	int cols = (num_cols+2);
	int loops = 5;
	while (loops > 0)
	{
		area_grid = std::vector<int>(rows * cols, 0);
		for (auto& it: ComponentVector)
		{
			x_f = (it->location().x() * 1000.0) / CORE_X_OFF;
			y_f = (it->location().y() * 1000.0) / CORE_Y_OFF;
			x_i = x_f;
			y_i = y_f;
			if (y_i > rows) continue;
			for (j = 0; j < component_width; ++j)
			{
				if ((x_i + j) > cols) break;
				area_grid[y_i * cols + (x_i + j)]++;
			}
		}

		for (i = 0; i < rows; ++i)
		{
			for (j = 0; j < cols; ++j)
			{
				printf("%2d ", area_grid[i * cols + j]++);
			}
			std::cout << std::endl;
		}
		for (auto& it: ComponentVector)
		{
			x_f = (it->location().x() * 1000.0) / CORE_X_OFF;
			y_f = (it->location().y() * 1000.0) / CORE_Y_OFF;
			x_i = x_f;
			y_i = y_f;
			i = ComponentHash[it->name()];
			
			d_x[i] *= 1.1; 
			d_y[i] *= 1.1;

			if (y_i == 0)
				d_y[i] += 0.576;
			else if (y_i > (rows-1))
				d_y[i] -= 0.576;

			if (x_i == 0)
				d_x[i] += 0.090;
			else if (x_i > (cols-1))
				d_x[i] -= 0.090;

		}
		L_BiCG->BiCG(x, d_x, 0.001);
		L_BiCG->BiCG(y, d_y, 0.001);
		int k=1;
		for (auto& it: ComponentVector)
		{
			it->set_location(Point<float>(x[k], y[k]));
			k++;
		}
		loops--;
		std::cout << std::endl;
	}
}
