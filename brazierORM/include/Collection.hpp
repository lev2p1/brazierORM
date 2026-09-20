/*
 * Copyright (c) 2026 Kirill Sergeev, Nikolay Sugonyako, Andrey Agarkov, Gleb Safyannikov
 * SPDX-License-Identifier: LGPL-3.0-or-later
 *
 * This file is part of brazier.
 *
 * brazier is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * brazier is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with brazier; if not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <vector>
#include <string>
#include <memory>
#include <nlohmann/json.hpp>

namespace brazier {

	using json = nlohmann::json;

	template <typename ModelType>
	class Collection : public std::vector<ModelType> {
	public:
		using std::vector<ModelType>::vector;

		Collection() = default;

		Collection(const std::vector<ModelType>& vec)
			: std::vector<ModelType>(vec) {}

		Collection(std::vector<ModelType>&& vec)
			: std::vector<ModelType>(std::move(vec)) {}

		bool save() {
			if (this->empty()) {
				return false;
			}
			return ModelType::saveMany(*this);
		}

		bool delete_() {
			if (this->empty()) {
				return false;
			}

			bool success = true;
			for (auto& item : *this) {
				try {
					item.delete_();
				}
				catch (const std::exception& e) {
					Logger::log("Failed to delete item: " + std::string(e.what()), "ERROR");
					success = false;
				}
			}
			return success;
		}

		json toJson() const {
			json j = json::array();
			for (const auto& item : *this) {
				j.push_back(item.toJson());
			}
			return j;
		}

		ModelType first() const {
			return this->empty() ? ModelType() : this->front();
		}

		ModelType last() const {
			return this->empty() ? ModelType() : this->back();
		}

		/*
			@brief Filters the collection based on a predicate function.
			@param predicate A function that takes a shared pointer to ModelType and returns a boolean.
			@return A new Collection containing only the elements that satisfy the predicate.
			@note This method does not modify the original collection; it returns a new filtered collection.
		*/
		Collection<ModelType> filter(std::function<bool(ModelType)> predicate) const {
			Collection<ModelType> result;
			std::copy_if(this->begin(), this->end(), std::back_inserter(result), predicate);
			return result;
		}

		bool all(std::function<bool(ModelType)> predicate) const {
			return std::all_of(this->begin(), this->end(), predicate);
		}

		bool any(std::function<bool(ModelType)> predicate) const {
			return std::any_of(this->begin(), this->end(), predicate);
		}

		ModelType find(std::function<bool(ModelType)> predicate) const {
			auto it = std::find_if(this->begin(), this->end(), predicate);
			return it != this->end() ? *it : ModelType();
		}

		std::vector<ModelType> toVector() const {
			return *this;
		}

		operator std::vector<ModelType>() const {
			return *this;
		}
	};
}