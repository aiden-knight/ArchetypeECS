#pragma once
#include <JSON/nlohmann/json.hpp>
#include "DefinesJSON.h"
#include <string>
/// <summary>
/// The following is an example of how to create a struct to be serialised into json
/// </summary>
struct ExampleStruct
{
	int name1;
	std::string name2;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ExampleStruct, name1, name2)

namespace JSONLoader
{
	using json = nlohmann::json;

	/// <summary>
	/// Loads json at path specified
	/// </summary>
	json Load(const char* path);

	/// <summary>
	/// Loads a NLOHMANN_DEFINE struct from a json file
	/// </summary>
	/// <param name="path">Path to the json file</param>
	/// <param name="name">The name under which the json file is found</param>
	/// <returns>Deserialised struct</returns>
	template<typename Struct>
	Struct Load(const char* path, const char* name)
	{
		json loaded = Load(path);
		Struct loadedObject;
		if (loaded.is_null()) return loadedObject;

		nlohmann::from_json(loaded[name], loadedObject);
		return loadedObject;
	}

	/// <summary>
	/// Loads a NLOHMANN_DEFINE struct from json object
	/// </summary>
	/// <param name="path">Path to the json file</param>
	/// <returns>Deserialised struct</returns>
	template<typename Struct>
	Struct Load(json json)
	{
		Struct loadedObject;
		if (json.is_null()) return loadedObject;

		nlohmann::from_json(json, loadedObject);
		return loadedObject;
	}

	/// <summary>
	/// Converts a NLOHMANN_DEFINE struct into json
	/// </summary>
	/// <param name="object">Struct to convert</param>
	/// <returns>The serialised json</returns>
	template<typename Struct>
	json ToJson(const Struct& object)
	{
		json newJson;
		nlohmann::to_json(newJson, object);
		return newJson;
	}
}

