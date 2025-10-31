#pragma once

#include <memory>
#include <string>
#include <unordered_map>

class Shader;

/** 
* Classe per gestire le shader da caricare nel renderer
*/

class ShaderManager {
public:
	ShaderManager() = default;
	~ShaderManager() = default;

	// se non esiste uno shader con quel nome lo carico nella mappa e ritorno
	std::shared_ptr<Shader> load(const std::string& name,
		const std::string& vertexPath,
		const std::string& fragmentPath);

	// ritorna la shader nella mappa
	std::shared_ptr<Shader> get(const std::string& name);

private:
	std::unordered_map<std::string, std::shared_ptr<Shader>> shadersMap;
	
};
