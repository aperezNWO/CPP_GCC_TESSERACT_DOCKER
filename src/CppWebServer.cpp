/*

    // WINDOWS
    g++ -std=c++17 -O3 CppWebServer.cpp -I. TFileManager.cpp Dijkstra.cpp SortBenchMark.cpp  Sudoku.cpp RegExManager.cpp -o cpp_web_server.exe -mconsole -lws2_32
 
    g++ -std=c++17 -O3 CppWebServer.cpp -o cpp_web_server.exe -mconsole -lws2_32
    
    // LINUX -- DOCKER (NO .EXE EXTENSION FOR DOCKER!!!)
    	// EDIT DOCKER FILE TO ADD NEEDED FILES
     		 COPY --from=builder /app/cdcatalog.xml /app/cdcatalog.xml  	
   			 COPY --from=builder /app/Algorithm.ini /app/Algorithm.ini
			 COPY --from=builder /app/cpp_web_server /app/cpp_web_server
        // COPY C++ COMMAND TO DOCKER FILE	
			 RUN  <COMMAND_LINE>		 
        g++ -std=c++17 -O3 CppWebServer.cpp -I. TFileManager.cpp Dijkstra.cpp SortBenchMark.cpp  Sudoku.cpp RegExManager.cpp -o cpp_web_server -lpthread 

        g++ -std=c++17 -O3 CppWebServer.cpp -I.  -o cpp_web_server -lpthread 

*/

#include "httplib.h"
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <map>
#include <filesystem>

std::map<std::string, std::string> configMap;

struct EndpointInfo {
    std::string endpointName;
    std::string endpointDescription;
};

// Key is a string identifier (e.g., "HEALTH_ENDPOINT")
std::unordered_map<std::string, EndpointInfo> endpointDictionary = {
    {"HEALTH_ENDPOINT"            , {"/health"                     , "Print all endpoints"}},
    {"PING_ENDPOINT"              , {"/zero"                       , "Render workaround"}},
    {"APP_VERSION_ENDPOINT"       , {"/getAppVersion"              , "Get Application Version"}},
    {"STD_VERSION_ENDPOINT"       , {"/getSTDVersion"              , "Get Standard C++ Version"}},
    {"SERVER_VERSION_ENDPOINT"    , {"/getServerVersion"           , "Get Http Server Version"}}
};

//
const char* GetCPPSTDVersion(long int cppVersion)	
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

// Get HTTP Server Version (cpp-httplib)
const char* GetCPPHttpVersion()
{
    #ifdef CPPHTTPLIB_VERSION
        return CPPHTTPLIB_VERSION;
    #else
        return "Unknown cpp-httplib Version";
    #endif
}

//
int ReadConfigFile(const char* fileName)
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
			configMap[key] = value;
		}
	}

	// Close the configuration file
	configFile.close();

	//
	return 0;
}	




////////////////////////////////////////////////////////////

int main(int argc, char* argv[]) {

	//
    httplib::Server svr;

    // HTTP Interceptor & Global CORS Middleware
    svr.set_pre_routing_handler([](const httplib::Request& req, httplib::Response& res) {
        // 1. Request Logging Interceptor
        std::cout << "[Interceptor] " << req.method << " " << req.path << std::endl;

        // 2. Global CORS Header Policy
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Origin, Content-Type, Accept, Authorization");
        res.set_header("Access-Control-Allow-Credentials", "true");

        // 3. Handle browser preflight OPTIONS requests automatically
        if (req.method == "OPTIONS") {
            res.status = 204; // No Content
            return httplib::Server::HandlerResponse::Handled;
        }

        // Proceed to route handlers
        return httplib::Server::HandlerResponse::Unhandled;
    });
    
      // Ping / Zero Endpoint
    svr.Get(endpointDictionary["PING_ENDPOINT"].endpointName.c_str(), [](const httplib::Request&, httplib::Response& res) {
        res.status = 204;
    });

    // Server Diagnostics (Health Endpoint listing all routes)
    svr.Get(endpointDictionary["HEALTH_ENDPOINT"].endpointName.c_str(), [&](const httplib::Request&, httplib::Response& res) {
    	//
		std::string webServerVersion = GetCPPHttpVersion();
        //
		std::string jsonResponse     = "{\n  \"server\": \"C++ Http Server 'cpp-httplib' - v[" + webServerVersion  + "] Working!\",\n  \"endpoints\": [\n";
        //
        size_t count = 0;
        for (const auto& pair : endpointDictionary) {
            const std::string& key = pair.first;
            const EndpointInfo& info = pair.second;
            
            std::string comma = (++count < endpointDictionary.size()) ? "," : "";

            jsonResponse += "    {\n";
            jsonResponse += "      \"key\": \"" + key + "\",\n";
            jsonResponse += "      \"path\": \"" + info.endpointName + "\",\n";
            jsonResponse += "      \"description\": \"" + info.endpointDescription + "\"\n";
            jsonResponse += "    }" + comma + "\n";
        }
        
        jsonResponse += "  ]\n}";
        
        res.set_content(jsonResponse, "application/json");
        res.status = 200;
    });
    
    // Get Standard C++ Version
    svr.Get(endpointDictionary["STD_VERSION_ENDPOINT"].endpointName.c_str(), [](const httplib::Request&, httplib::Response& res) {
    		std::string jsonResponse  = GetCPPSTDVersion(__cplusplus); 
            res.set_content(jsonResponse, "application/json");
            res.status = 200;
    });
    
        
    // Get Application Version
    svr.Get(endpointDictionary["APP_VERSION_ENDPOINT"].endpointName.c_str(), [](const httplib::Request&, httplib::Response& res) {
    	
    		//
			ReadConfigFile("Algorithm.ini");

			//
    		std::string jsonResponse  = configMap["DLL_VERSION"]; 
    		
    		//
            res.set_content(jsonResponse, "application/json");
            res.status = 200;
    });
    
	// Get Http Server Version
    svr.Get(endpointDictionary["SERVER_VERSION_ENDPOINT"].endpointName.c_str(), [](const httplib::Request&, httplib::Response& res) {
    		std::string jsonResponse  = GetCPPHttpVersion();
            res.set_content(jsonResponse, "application/json");
            res.status = 200;
    });
    
    int port = 8080;
    if (getenv("PORT")) {
        port = std::stoi(getenv("PORT"));
    }

    std::cout << "C++ REST Server listening on port " << port << "...\n";
    svr.listen("0.0.0.0", port);

    return 0;
}
