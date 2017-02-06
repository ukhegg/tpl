#include <tpl/object_id.hpp>
#include <algorithm>
#include <sstream>

namespace tpl {
	object_id::object_id() {}

	bool operator==(object_id const &lhs, object_id const &rhs) {
		return lhs.index_ == rhs.index_;
    }
	 
    bool operator!=(object_id const &lhs, object_id const &rhs) {
        return !(lhs == rhs);
    }

    bool operator<(object_id const &lhs, object_id const &rhs) {
		auto common_index_count = std::min<size_t>(lhs.index_.size(), rhs.index_.size());

		for(size_t index = 0; index < common_index_count; ++index)
		{
			if (lhs.index_[index] != rhs.index_[index]) throw std::invalid_argument("packet ids not comparable");
		}
		return  lhs.index_.size() < rhs.index_.size();
    }

    bool operator<=(object_id const &lhs, object_id const &rhs) {
        return !(rhs < lhs);
    }

    bool operator>(object_id const &lhs, object_id const &rhs) {
        return rhs < lhs;
    }

    bool operator>=(object_id const &lhs, object_id const &rhs) {
        return !(lhs < rhs);
    }

	std::ostream& operator<<(std::ostream& os, const object_id& obj)
	{
		os << obj.name_ << '<';
		bool is_first = true;
		for (auto index : obj.index_)
		{
			if (is_first) is_first = false;
			else os << ',';
			os << index;
		}
		os << '>';
		return os;
	}

	object_id::object_id(object_id const& parent, uint32_t index, std::string const& name)
			: index_(parent.index_), name_(name)
	{
		this->index_.push_back(index);
	}

    const std::string &object_id::name() const {
        return name_;
    }

    objects_id_repository& objects_id_repository::instance()
	{
		return instance_;
	}

	objects_id_repository objects_id_repository::instance_;

	object_id objects_id_repository::register_type(object_id const& parent, std::string const& name)
	{
		auto& node = this->find_node(parent);
		for(auto const& c : node.child_nodes)
		{
			if (c->value.name() == name) return  c->value;
		}

		auto new_node = std::make_shared<objects_tree_node>();
		new_node->value = object_id(node.value, static_cast<uint32_t>(node.child_nodes.size()), name);
		node.child_nodes.push_back(new_node);
		return new_node->value;
	}

	object_id const& objects_id_repository::root_object_id()
	{
		return instance().objects_tree_root_.value;
	}

	objects_id_repository::objects_id_repository()
	{
	}

	objects_id_repository::objects_id_repository(objects_id_repository const& rhv)
	{
	}

	objects_id_repository::objects_tree_node& objects_id_repository::find_node(object_id const& index)
	{
		auto* current = &this->objects_tree_root_;
		for(auto i : index.index_)
		{
			if (current->child_nodes.size() <= i)
			{
				std::ostringstream oss;
				oss << index << " not found";
				throw std::out_of_range(oss.str().c_str());
			}
			current = current->child_nodes.at(i).get();
			if (current == nullptr) throw std::runtime_error("node at given index is nullptr");
		}
		return *current;
	}

	object_id const& objects_id_repository::register_type(std::type_info const& object_type,
                                                          boost::optional<std::type_info const&> parent_type,
                                                          std::string const& name)
	{
		try
		{
			auto& parent_id = parent_type == boost::none
				                  ? this->objects_tree_root_.value
				                  : this->id_by_type_.at(std::type_index(*parent_type));
			auto id = this->register_type(parent_id, name);
			return this->id_by_type_[std::type_index(object_type)] = id;
		}
		catch (std::out_of_range const&)
		{
			throw std::invalid_argument("parent class not registered");
		}
	}

	contains_object_id::~contains_object_id()
	{
	}


	object_id const& contains_object_id::get_id() const
	{
		if (this->lazy_id_ == boost::none)
		{
			this->lazy_id_ = this->register_self();
		}
		return this->lazy_id_.value();
	}

	contains_object_id::object_info::object_info()
	{
	}

	contains_object_id::object_info::object_info(const object_id& parent_id, const std::string& name): parent_id(parent_id),
	                                                                                                   name(name)
	{
	}

	object_id contains_object_id::register_self() const
	{
		auto self_info = this->get_object_info();
		return objects_id_repository::instance().register_type(self_info.parent_id, self_info.name);
	}
}
