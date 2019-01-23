#include "Loader.h"
#include <memory>
using std::unique_ptr;


const string OFF_COMMENT = "#";
const string OBJ_COMMENT = "#";

inline void AbsorbMin(vec3& me, const vec3& other) {
	me.x = min(me.x, other.x);
	me.y = min(me.y, other.y);
	me.z = min(me.z, other.z);
}
inline void AbsorbMax(vec3& me, const vec3& other) {
	me.x = max(me.x, other.x);
	me.y = max(me.y, other.y);
	me.z = max(me.z, other.z);
}
inline bool TryGetNonComment(fstream& file, string& ret,const string& comment) {
	string line;
	
	while (file.peek() != EOF) {
		getline(file, line);
		String::ltrim(line);
		
		int pos = line.find(comment);
		//cout << "Line -> " << line << endl;
		if (pos != string::npos) 
		{
			line = line.substr(0, pos);
		}
		//cout << "Line substr -> " << line << endl;

		if (line.empty()) { continue; }
			
		//cout << "Line ret -> " << line << endl;

		ret = line;
		return true;
	}
	return false;
}


void Loader::Load(string path, void func(ObjectRenderer*) = nullptr) {
	ObjectRenderer *_re = new ObjectRenderer();
	_re->path = path;

	std::thread* t;

	if (String::EndsWith(path, ".obj"))
	{
		t = new thread(&Loader::FromOBJ, _re, func);

	}
	else if(String::EndsWith(path, ".off"))
	{
		t = new thread(&Loader::FromOFF, _re, func);

	}
	else if (String::EndsWith(path, ".soff"))
	{
		t = new thread(&Loader::FromSOFF, _re, func);
		cout << "soff " << endl;
	}
	else {
		cout << "Path " << path << " extention is not allowed\n";
		return;
	}

}

ObjectRenderer* Loader::FromSOFF(ObjectRenderer* _re, void func(ObjectRenderer*) = nullptr) {

	fstream file;
	string token, v1, v2, v3;

	file.open(_re->path, std::ios::in);

	file >> token;

	if (token != "SOFF")
	{
		file.close();
		return nullptr;
	}
	else
	{
		file >> token;
		int mNumOfVertices = atoi(token.c_str());

		for (int i = 0; i < mNumOfVertices; i++)
		{
			file >> v1;
			file >> v2;
			file >> v3;
			_re->mVertices.push_back(glm::vec3((atof(v1.c_str())), (atof(v2.c_str())), (atof(v3.c_str()))));
		}

		for (int ii = 0; ii < mNumOfVertices; ii += 3)
		{
			_re->mTriangles.push_back({ (TType)ii, (TType)(ii + 1),(TType)(ii + 2) });
		}

		file.close();
		func(_re);
		return _re;
	}
}

ObjectRenderer* Loader::FromOBJ(ObjectRenderer* _re, void func(ObjectRenderer*) = nullptr) {
	vector<string> tokens;
	fstream file;
	string line;
	file.open(_re->path, std::ios::in);

	while (TryGetNonComment(file, line, OBJ_COMMENT)) {
		tokens = String::Split(" ", line);
		const string type = tokens[0];
		const int initValues = 1;

		if (type == "v") {
			vec3 vec(stof(tokens[initValues]), stof(tokens[initValues + 1]), stof(tokens[initValues + 2]));

			AbsorbMin(_re->minsBB, vec);
			AbsorbMax(_re->maxsBB, vec);

			_re->mVertices.push_back(vec);
			continue;
		}


		if (type == "f")
		{
			vector<int> indexes;
			indexes.reserve(tokens.size() - 1);

			// estos ifs se pueden mejorar sacando el for pero que ladilla, no es el momento ahora
			if (tokens[1].find("//") != string::npos) { // si son duplas estan separados por "//"
				//cout << "Reading dupla "<< line<<endl;
				for (int ii = initValues, iiEnd = tokens.size(); ii < iiEnd; ii++)
				{
					indexes.push_back(stoi(String::Split("//", tokens[ii])[0]));
				}
			}
			else if (tokens[1].find("/") != string::npos)
			{ // si son tripletas estan separados por "/"
				//cout << "Reading tripleta " << line << endl;

				for (int ii = initValues, iiEnd = tokens.size(); ii < iiEnd; ii++)
				{
					indexes.push_back(stoi(String::Split("/", tokens[ii])[0]));
				}

			}
			else {
				//cout << "Reading individual " << line << endl;

				for (int ii = initValues, iiEnd = tokens.size(); ii < iiEnd; ii++)
				{
					indexes.push_back(stoi(tokens[ii]));
				}
			}

			for (int ii = 2, iiEnd = indexes.size(); ii < iiEnd; ii++) {
				_re->mTriangles.push_back({ (TType)indexes[ii - 2], (TType)indexes[ii - 1], (TType)indexes[ii] });
			}

			continue;
		}
	}

	size_t numVertex = _re->mVertices.size();
	for (auto& triangle : _re->mTriangles) {
		for (auto& index : triangle) {
			if (index > 0) {
				index--;
			}
			if (index < 0) {
				index = (TType)(numVertex + index);
			}
		}
	}

	file.close();
	_re->CalculateNormals();
	func(_re);
	return _re;
}
// Done, no tocar
ObjectRenderer* Loader::FromOFF(ObjectRenderer* _re, void func(ObjectRenderer*) = nullptr) {
	//first line : number of vertices, number of faces, number of edges(can be ignored)
	//list of vertices : X, Y and Z coordinates
	//list of faces : number of vertices, followed by the sequence number of the composing vertices(indexed from zero)
	//Optional face color information can follow the vertex list

	vector<string> tokens;
	fstream file;
	string line;

	int nVertex, nFaces; // , nEdges;

	file.open(_re->path, std::ios::in);

	TryGetNonComment(file, line, OFF_COMMENT);

	if (line != "OFF") {
		cout << "COÑO MANO QUE ES CUMPLE EL FORMATO VALE\n";
	}

	TryGetNonComment(file, line, OFF_COMMENT);

	tokens = String::Split(" ", line);

	nVertex = stoi(tokens[0]);
	nFaces = stoi(tokens[1]);
	//nEdges = stoi(tokens[2]);
	//cout << "File nVertex " << nVertex << " |" << "File nTriangles " << nFaces << endl;

	while (nVertex--)
	{
		TryGetNonComment(file, line, OFF_COMMENT);
		tokens = String::Split(" ", line);
		vec3 vec = vec3(stof(tokens[0]), stof(tokens[1]), stof(tokens[2]));

		AbsorbMin(_re->minsBB, vec);
		AbsorbMax(_re->maxsBB, vec);

		_re->mVertices.push_back(vec);
	}

	while (nFaces--)
	{
		TryGetNonComment(file, line, OFF_COMMENT);
		tokens = String::Split(" ", line);

		int nVertexFace = stoi(tokens[0]);

		for (int ii = 2, iiEnd = nVertexFace; ii < iiEnd; ii++)
		{
			_re->mTriangles.push_back({ (TType)stoi(tokens[1]), (TType)stoi(tokens[ii]), (TType)stoi(tokens[ii + 1]) });
		}
	}

	//cout << "nVertex " <<_re->renderer.mVertices.size() << " |  renderer.mTriangles " << _re->renderer.mTriangles.size() << endl;

	file.close();
	_re->CalculateNormals();

	func(_re);
	return _re;

}