#pragma once
#include "EngineTypes.h"

// System Libs
#include <functional>

template<typename... Args>
class EEvents {
public:
	// Adding a function into callbacks array
	EUi8 Bind(const std::function<void(Args...)>& callback) {
		// Make a unique structure or ECallbackNode
		auto newNode = TMakeUnique<ECallbackNode>();
		// Move the structure into the unique struct
		newNode->callback = std::move(callback);
		 
		// This will give the index for the callback
		EUi8 id = 0;
		if (m_callbackNodes.size() > 0) {
			// Find an id to assign the callback node
			// Starting with 1 ID
			EUi8 potentialID = 1;

			// Looping through each number to test if an ID exists
			while (id == 0) {
				// Default the found as true
				bool foundID = false;
				// If the node has that ID set found to false and break from loop
				for (const auto& node : m_callbackNodes) {
					if (node->id == potentialID) {
						foundID = true;
						break;
					}
				}

				// If no ID matches the potential ID, break and set ID
				if (!foundID) {
					id = potentialID;
					break;
				} 

				// If an ID was found that matched, check the next potential ID
				++potentialID;
			}
		}
		else {
			id = 0;
		}

		newNode->id = id;

		// Add the new unique callback into the array
		m_callbackNodes.push_back(std::move(newNode));

		return id;
	}

	// Run all functions bound to this event listener
	void Run(const Args&... args) {
		// Loop through all of the stored functions
		for (const auto& node : m_callbackNodes) {
			// Run each function with the arguments
			node->callback(args...);
		}
	}

	// Unbind a function based on the index
	// Get the index from the initial bind
	void Unbind(const EUi8& index) {
		std::erase_if(m_callbackNodes.begin(), m_callbackNodes.end(),
			[index](const ECallbackNode& node) {
				return node->id == index;
			}
		);
	}

private:
	struct ECallbackNode {
		std::function<void(Args...)> callback;
		EUi8 id;
	};

	// Create an array of functions and pass in arguments
	// to be run when the event runs
	TArray<TUnique<ECallbackNode>> m_callbackNodes;
};

typedef EEvents<> EEventsVoid;
