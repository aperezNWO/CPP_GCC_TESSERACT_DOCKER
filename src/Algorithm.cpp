///////////////////////////////////////////////////////////////////////////
// ALGORITHM.DLL
///////////////////////////////////////////////////////////////////////////
/*

		-- [_] TOOLCHAIN :  "C:\msys64\ucrt64\bin\g++.exe"
*/

///////////////////////////////////////////////////////////////////////////
// VISUAL STUDIO
///////////////////////////////////////////////////////////////////////////
/*
	1) SELECCIONAR ARCHIVO *.CPP DLL
	
	2) TERMINAL > RUN BUILD TASK > (3) (DLL) C/C++: g++.exe build active file
	3) SE GENERA ARCHIVO "ALGORITMO.DLL" STATIC DLL X64
	4) DISTRIBUIR LOS SIGUIENTES ARCHIVOS  EN CARPETA "/_DIST":

		a) Algorithm.dll
		b) Algorithm.ini
		c) cdCatalog.xml

*/

///////////////////////////////////////////////////////////
// COMMAND LINE
///////////////////////////////////////////////////////////
/* 

  // Algorithm.dll
 "C:\msys64\ucrt64\bin\g++.exe" -shared -static -static-libgcc -static-libstdc++ -fdiagnostics-color=always -g Algorithm.cpp include\Dijkstra.cpp include\RegExManager.cpp include\SortBenchMark.cpp include\Sudoku.cpp include\TFileManager.cpp -o Algorithm.dll -Iinclude -Wl,--subsystem,windows -DALGORITHM_EXPORTS -m64
 
 // Algoritym.o 
 "C:\msys64\ucrt64\bin\g++.exe" -c Algorithm.cpp -o Algorithm.o -Iinclude -Wall -Wextra -fdiagnostics-color=always -g -m64 -DALGORITHM_EXPORTS

// TFileManager.o
"C:\msys64\ucrt64\bin\g++.exe" -c include\TFileManager.cpp -o TFileManager.o -Iinclude -Wall -Wextra -fdiagnostics-color=always -g -m64 -DALGORITHM_EXPORTS

// RegexManager.o
"C:\msys64\ucrt64\bin\g++.exe" -c include\RegExManager.cpp -o RegExManager.o -Iinclude -Wall -Wextra -fdiagnostics-color=always -g -m64 -DALGORITHM_EXPORTS

// Sudoku.o
"C:\msys64\ucrt64\bin\g++.exe" -c include\Sudoku.cpp -o Sudoku.o -Iinclude -Wall -Wextra -fdiagnostics-color=always -g -m64 -DALGORITHM_EXPORTS
 
// Dijkstra.o 
"C:\msys64\ucrt64\bin\g++.exe" -c include\Dijkstra.cpp -o Dijkstra.o -Iinclude -Wall -Wextra -fdiagnostics-color=always -g -m64 -DALGORITHM_EXPORTS

// SortBenchMark.o
"C:\msys64\ucrt64\bin\g++.exe" -c include\SortBenchMark.cpp -o SortBenchMark.o -Iinclude -Wall -Wextra -fdiagnostics-color=always -g -m64 -DALGORITHM_EXPORTS

 // PUT TOGETHER IN A LIBALGORITHM.A FILE
 ar rcs libAlgorithm.a Algorithm.o TFileManager.o RegExManager.o Sudoku.o Dijkstra.o SortBenchMark.o
 
 
*/
///////////////////////////////////////////////////////////
// MAKE
///////////////////////////////////////////////////////////
/* 

mingw32-make --always-make --debug=v

make.bat

dumpbin /exports Algorithm.dll


gcc version 15.1.0 (Rev1, Built by MSYS2 project)

GCC 10.x and later : Default is C++17 .



*/
///////////////////////////////////////////////////////////

#include "include\Algorithm.h"
#include "include\Dijkstra.h"
#include "include\SortBenchMark.h"
#include "include\RegExManager.h"
#include "include\Sudoku.cpp"



	Algorithm::Algorithm(bool readConfigFile)
	{
		 //
		 if (readConfigFile)
		 {
		 	this->ReadConfigFile("Algorithm.ini");
		 }

	}
	//
	Algorithm::~Algorithm()
	{
		//
	}
	//
	string Algorithm::HtmlEncode(const std::string& input)
	{
		//
		std::ostringstream encoded;
		//
		for (char c : input) {
			//
			if (std::isspace(c))
			{
				encoded << "&nbsp;";
			}
			//
			switch (c) {
				case '&': encoded << "&amp;";
					break;
				case '<': encoded << "&lt;";
					break;
				case '>': encoded << "&gt;";
					break;
				case '\"': encoded << "&quot;";
					break;
				case '\'': encoded << "&apos;";
					break;
				case '\n': encoded << "<br/>";
					break;
				case '\t': encoded << "&nbsp;";
					break;
				case '[': encoded << "<mark>";
					break;
				case ']': encoded << "</mark>";
					break;
				default: encoded << c;
			}
		}
		return encoded.str();
	}
	//
	string Algorithm::StringTrim(const std::string& str)
	{
		//
		size_t start = str.find_first_not_of(" \t\n\r");   // Find the first non-whitespace character
		size_t end   = str.find_last_not_of(" \t\n\r");    // Find the last non-whitespace character
		//
		if (start == std::string::npos || end == std::string::npos) {
			// The string is empty or contains only whitespaces
			return "";
		}
		//
		return str.substr(start, end - start + 1);
	};
	//
	vector<string> Algorithm::StringSplit(const char* p_inputString, std::string p_delimiter)
	{
		//
		std::string s(p_inputString);
		//
		std::vector<std::string> outputArr;
		size_t pos = 0;
		std::string token;
		//
		while ((pos = s.find(p_delimiter)) != std::string::npos) {
			token = s.substr(0, pos);
			s.erase(0, pos + p_delimiter.length());
			outputArr.push_back(token);
		}
		outputArr.push_back(s);
		//
		return outputArr;
	};
	//
	int Algorithm::ReadConfigFile(const char* fileName)
	{
		 // Open the configuration file
		std::ifstream configFile(fileName);

		// Check if the file is opened successfully
		if (!configFile.is_open()) {
			std::cerr << "Error opening the configuration file." << std::endl;
			return 1;
		}

		// Read the file line by line
		std::string line;
		while (std::getline(configFile, line)) {
			// Skip empty lines or lines starting with '#' (comments)
			if (line.empty() || line[0] == '#') {
				continue;
			}

			// Split the line into key and value
			std::istringstream iss(line);
			std::string key, value;
			if (std::getline(iss, key, '=') && std::getline(iss, value))
			{
				// Trim leading and trailing whitespaces from key and value
				key.erase(0, key.find_first_not_of(" \t"));
				key.erase(key.find_last_not_of(" \t") + 1);
				value.erase(0, value.find_first_not_of(" \t"));
				value.erase(value.find_last_not_of(" \t") + 1);

				// Insert key-value pair into the map
				this->configMap[key] = value;
			}
		}

		// Close the configuration file
		configFile.close();

		//
		return 0;
	}

	void Algorithm::ReplaceAll(std::string &str, const std::string &from, const std::string &to) 
	{
		//
		size_t startPos = 0;
		//
		while ((startPos = str.find(from, startPos)) != std::string::npos) {
			str.replace(startPos, from.length(), to);
			startPos += to.length(); // Move to the next position after replacement
		}
	}
	//
	const char* Algorithm::_GetDLLVersion()
	{
		//
		return this->configMap["DLL_VERSION"].c_str();
	}
	//
	const char* Algorithm::GetCPPSTDVersion(long int cppVersion)	
	{
		switch (cppVersion) {
		        case 199711L: return "C++98/C++03";
		        case 201103L: return "C++11";
		        case 201402L: return "C++14";
		        case 201703L: return "C++17";
		        case 202002L: return "C++20";
		        case 202302L: return "C++23";
		        default: return "Unknown C++ Standard";
		}
	}

	///////////////////////////////////////////////////////////////////////////
	// ENTRY POINTS
	///////////////////////////////////////////////////////////////////////////

	// DIJKSTRA
	DLL_EXPORT const char* Dijkstra_GenerateRandomVertex_CPP(int p_vertexSize, int p_sourcePoint)
	{
	    // Create a unique_ptr for Dijkstra
	    std::unique_ptr<Dijkstra> uniquePtr = std::make_unique<Dijkstra>();
	
	    // Generate the response
	    static std::string response; 
		
		response = uniquePtr->GetRandomPoints(p_vertexSize, p_sourcePoint);
	
	    // Return the pointer to the dynamically allocated memory
	    return response.c_str();
	}
	// SORT BENCHMARK
	DLL_EXPORT const char*  SortBenchMark_GetSort_CPP(int p_sortAlgoritm, const char* p_unsortedList)
	{
	    if (!p_unsortedList || !p_sortAlgoritm) {
	        throw std::invalid_argument("Invalid input parameters");
	    }
	
	    // Create SortBenchMark object on the stack
	    FormatType format = FormatType::HTML; // HTML
	    SortBenchMark benchmark(p_unsortedList, format);
	
	    // Perform sorting and store the result in a static string
	    static std::string jsonResponse;
	    jsonResponse = benchmark.GetSort(p_sortAlgoritm);
	
	    // Return the pointer to the static string
	    return jsonResponse.c_str();
	};
	// SORT BENCHMARK
	DLL_EXPORT const char* SortBenchMark_GetSort_CPP_JSON(int p_sortAlgoritm, const char* p_unsortedList)
	{
	    if (!p_unsortedList || !p_sortAlgoritm) {
	        throw std::invalid_argument("Invalid input parameters");
	    }
	
	    // Create SortBenchMark object on the stack
	    FormatType format = FormatType::JSON; // JSON
	    SortBenchMark benchmark(p_unsortedList, format);
	
	    // Perform sorting and store the result in a static string
	    static std::string jsonResponse;
	    jsonResponse = benchmark.GetSort(p_sortAlgoritm);
	
	    // Return the pointer to the static string
	    return jsonResponse.c_str();
	}
	// REGULAR EXPRESSIONS
	DLL_EXPORT const char*  RegExManager_RegExEval(char* p_tagSearch, char* p_textSearch)
	{
		//
		std::unique_ptr<RegExManager> uniquePtr = std::make_unique<RegExManager>();
		//
		static std::string response; 
		response  = uniquePtr->RegExEval(p_tagSearch, p_textSearch);
		
		//
		return response.c_str();
	}
	// SUDOKU
	DLL_EXPORT const char*  Sudoku_Generate_CPP()
	{
		//
		const static int   N = 9;
		int                K = 20;

		//
		std::unique_ptr<SudokuGenerator> uniquePtr = std::make_unique<SudokuGenerator>(N, K);
		string           str_matrix                = uniquePtr->Run();

		//
		return str_matrix.c_str();
	}
	// SUDOKU
	DLL_EXPORT const char*  Sudoku_Solve_CPP(char* p_matrix)
	{
	    //
		Algorithm*   algorithm     = new Algorithm(false);
		string       str_p_matrix  = p_matrix;
		FileManager* fileManager   = new FileManager();

		//
		std::map<std::string, std::string> replaceMap;

		// Inserting values into the map
		replaceMap["["]      = "";
		replaceMap["]"]      = "";
		replaceMap["},"]     = "|";
		replaceMap["{"]      = "";
		//replaceMap["}"]      = "";  // ERROR EN ESTE REEMPLAZO
		// SE DEBE REEMPLAZAR ESPECIFICMENTE MAS ABAJO

		// Iterating through the map
		for (const auto& pair : replaceMap) {
			//
			algorithm->ReplaceAll(str_p_matrix, pair.first, pair.second);
		}
		//
		algorithm->ReplaceAll(str_p_matrix, "}", "");

		//
		const char* str_p_matrix_c_str     = str_p_matrix.c_str();
		//fileManager->SaveLineToFile("\nSudoku To Solve\n","SudokuGenerated.txt");

		//
		const static int   N          = 9;
		int                grid[N][N] =
			{
				{-1, -1, -1, -1, -1, -1, -1, -1, -1},
				{-1, -1, -1, -1, -1, -1, -1, -1, -1},
				{-1, -1, -1, -1, -1, -1, -1, -1, -1},
				{-1, -1, -1, -1, -1, -1, -1, -1, -1},
				{-1, -1, -1, -1, -1, -1, -1, -1, -1},
				{-1, -1, -1, -1, -1, -1, -1, -1, -1},
				{-1, -1, -1, -1, -1, -1, -1, -1, -1},
				{-1, -1, -1, -1, -1, -1, -1, -1, -1},
				{-1, -1, -1, -1, -1, -1, -1, -1, -1} 
			};

		//
		vector<string>   str_p_matrix_rows = algorithm->StringSplit(str_p_matrix_c_str,"|");
		//
		int i = 0;
		//
		for (string row : str_p_matrix_rows) {
			//
			//fileManager->SaveLineToFile(row,"SudokuGenerated.txt");
			//
			int j = 0;
			//				
			vector<string>   str_p_matrix_cols = algorithm->StringSplit(row.c_str(),",");
			//
			for (string col : str_p_matrix_cols) {
				//
				int num = stoi(col);
				//
				grid[i][j] = num;
				//
				j++;	
			}
			//
			i++;
		}

		//
		std::unique_ptr<SudokuSolver> uniquePtrSolve = std::make_unique<SudokuSolver>();
		string           str_matrix                  = uniquePtrSolve->Solve(grid);

		//
		//fileManager->SaveLineToFile("\nSudoku Solved \n","SudokuSolved.txt");
		//
		return str_matrix.c_str();
	}
	// APP_VERSION
	DLL_EXPORT const char* GetDLLVersion()
	{
		//
		std::unique_ptr<Algorithm> uniquePtr = std::make_unique<Algorithm>(true);
		//
		return uniquePtr->_GetDLLVersion();
	}

	// C++ VERSION
	DLL_EXPORT const char* GetCPPSTDVersion()
	{
		Algorithm *algorithm = new Algorithm(false);
		
		return algorithm->GetCPPSTDVersion(__cplusplus);	
	}

	///////////////////////////////////////////////////////////////////////////
