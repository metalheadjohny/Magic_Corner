#pragma once
#include <SFML\Network.hpp>
#include <string>
#include <iostream>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"


struct HostedGame {
	std::string ID, ip, port, name, createdAt;

	HostedGame(std::string ID, std::string ip, std::string port, std::string name, std::string createdAt) : 
		ID(ID),
		ip(ip),
		port(port),
		name(name), 
		createdAt(createdAt) {}
};

class MatchMaker{

	sf::Http http;
	std::string oldId = "-1";	//for dropping the hosting after you are done

public:

	bool hosting = false;
	
	MatchMaker();

	void setHost(std::string serverUrl = "http://127.0.0.1:80") {
		http.setHost(serverUrl);
	}

	~MatchMaker();

	sf::Http::Response sendRequest(const sf::Http::Request::Method& method, const std::string& uri, 
		const std::vector<std::pair<std::string, std::string>>& params) {

		sf::Http::Request request;
		request.setMethod(method);
		request.setUri(uri);

		for (auto p : params)
			request.setField(p.first, p.second);

		if (method == sf::Http::Request::Method::Post) {

			std::string bodyString = "";

			for (auto p : params)
				bodyString.append(p.first + "=" + p.second + "&");
			
			if (bodyString.length() != 0)
				bodyString.pop_back();

			request.setBody(bodyString);
		}
		
		request.setHttpVersion(1u, 1u);

		auto response = http.sendRequest(request);

		return response;
	}


	void parseResponse(sf::Http::Response response, std::string& result) {
		//std::cout << response.getStatus() << std::endl;
		//std::cout << "HTTP version: " << response.getMajorHttpVersion() << "." << response.getMinorHttpVersion() << std::endl;
		//std::cout << "Content-Type header:" << response.getField("Content-Type") << std::endl;
		//std::cout << "body: " << response.getBody() << std::endl;
		result = response.getBody();
	}

	std::string host() {

		std::vector <std::pair<std::string, std::string>> params;
		params.push_back(std::make_pair("hosting", "whee"));
		params.push_back(std::make_pair("ip", "127.0.0.1"));
		params.push_back(std::make_pair("port", std::to_string(36963)));
		params.push_back(std::make_pair("name", "Host#" + random_string(5)));

		sf::Http::Response response = sendRequest(sf::Http::Request::Method::Post, "", params);
		parseResponse(response, oldId);

		hosting = true;

		return oldId;
	}



	void unhost() {

		std::vector <std::pair<std::string, std::string>> params;
		params.push_back(std::make_pair("stopHosting", oldId));
		params.push_back(std::make_pair("ip", "127.0.0.1"));

		sf::Http::Response response = sendRequest(sf::Http::Request::Method::Post, "", params);
		std::string responseString;
		parseResponse(response, responseString);

		std::cout << responseString << std::endl;

		hosting = false;
	}



	std::vector<HostedGame> listGames() {

		std::vector <std::pair<std::string, std::string>> params;
		params.push_back(std::make_pair("listGames", ""));

		sf::Http::Response response = sendRequest(sf::Http::Request::Method::Post, "", params);
		std::string responseString;
		parseResponse(response, responseString);

		std::vector<HostedGame> result;

		if (responseString == "0") {
			return result;
		}
		else {

			rapidjson::Document doc;
			doc.Parse(responseString.c_str());

			for (rapidjson::Value::ConstValueIterator itr = doc.Begin(); itr != doc.End(); ++itr) {

				const rapidjson::Value& hostedGame = *itr;
				
				if (hostedGame.IsObject()) {
					HostedGame hg(
						hostedGame.FindMember("ID")->value.GetString(),
						hostedGame.FindMember("ip")->value.GetString(),
						hostedGame.FindMember("port")->value.GetString(),
						hostedGame.FindMember("name")->value.GetString(),
						hostedGame.FindMember("created_at")->value.GetString()
					);
					result.push_back(hg);
				}	
			}
		}
		return result;
	}


	std::string random_string(size_t length){

		auto randchar = []() -> char
		{
			const char charset[] =
				"0123456789"
				"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
				"abcdefghijklmnopqrstuvwxyz";
			const size_t max_index = (sizeof(charset) - 1);
			return charset[rand() % max_index];
		};
		std::string str(length, 0);
		std::generate_n(str.begin(), length, randchar);
		return str;
	}
};

