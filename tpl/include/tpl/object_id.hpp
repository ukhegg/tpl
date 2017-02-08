#pragma once
#include "tpl/tpl.hpp"
#include <typeinfo>
#include <vector>
#include <memory>
#include <ostream>
#include <map>
#include <typeindex>
#include <boost/optional/optional.hpp>

namespace tpl {
    struct TPL_API object_id {
        object_id();

        friend TPL_API bool operator==(object_id const &lhs, object_id const &rhs);

        friend TPL_API bool operator!=(object_id const &lhs, object_id const &rhs);

        friend TPL_API bool operator<(object_id const &lhs, object_id const &rhs);

        friend TPL_API bool operator<=(object_id const &lhs, object_id const &rhs);

        friend TPL_API bool operator>(object_id const &lhs, object_id const &rhs);

        friend TPL_API bool operator>=(object_id const &lhs, object_id const &rhs);

		friend TPL_API std::ostream& operator<<(std::ostream& os, const object_id& obj);

        const std::string &name() const;

    private:
		friend class objects_id_repository;
		std::vector<uint32_t> index_;
		std::string name_;

		object_id(object_id const& parent, uint32_t index, std::string const& name);
    };

	template <class t_object>
	struct object_registrator
	{
		inline static object_id const& get_object_id();
	};

	class TPL_API objects_id_repository
	{
	public:
		static objects_id_repository& instance();

		object_id register_type(object_id const& parent, std::string const& name);

		static object_id const& root_object_id();

		template<class t_object>
		object_id const& register_type(std::string const& name)
		{
			return this->register_type(typeid(t_object), boost::none, name);
		}

		template<class t_object, class t_base>
		object_id const& register_subtype(std::string const& name) 
		{
			static boost::optional<object_id> t_object_id;
			if(t_object_id == boost::none)
			{
				t_object_id = this->register_type(this->id_of<t_base>(), name);
			}
			return t_object_id.value();
		}

		template<class t_object>
		static object_id const& id_of()
		{
			return object_registrator<t_object>::get_object_id();
		}
	private:
		struct objects_tree_node
		{
			object_id value;
			std::vector<std::shared_ptr<objects_tree_node>> child_nodes;
		};

		objects_tree_node objects_tree_root_;
		std::map<std::type_index, object_id> id_by_type_;

		static objects_id_repository instance_;

		objects_id_repository();

		objects_id_repository(objects_id_repository const& rhv);

		objects_tree_node& find_node(object_id const& index);

		object_id const& register_type(std::type_info const& object_type,
		                               boost::optional<std::type_info const&> parent_type,
		                               std::string const& name);
	};
	struct TPL_API root_object{};
	template<> struct TPL_API object_registrator<root_object> {
		static object_id const& get_object_id() {
				return objects_id_repository::root_object_id();
		}
	};

	struct TPL_API contains_object_id
	{	
		virtual ~contains_object_id();

		object_id const& get_id() const;
	protected:
		struct object_info
		{
			object_info();

			object_info(const object_id& parent_id, const std::string& name);

			object_id parent_id;
			std::string name;
		};

		virtual object_info get_object_info() const = 0;

		template<class t_base>
		object_id get_id_of_base() const
		{
			return dynamic_cast<t_base const*>(this)->get_id();
		}
	private:
		mutable boost::optional<object_id> lazy_id_;

		object_id register_self() const;
	};
}

#define REGISTER_TPL_OBJECT(ObjectType, ObjectBaseType) \
namespace tpl{\
	template<> struct TPL_API object_registrator<ObjectType>{\
		inline static tpl::object_id const& get_object_id(){ \
			return tpl::objects_id_repository::instance().register_subtype<ObjectType, ObjectBaseType>( #ObjectType ); \
		}\
	};\
}
