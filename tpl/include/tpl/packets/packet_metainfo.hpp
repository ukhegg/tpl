#pragma once

#include "tpl/tpl.hpp"
#include "tpl/packets/transport/transport_ports.hpp"
#include <boost/optional.hpp>
#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/asio/ip/address.hpp>
#include "tpl/utils/mac_address.hpp"
#include <memory>
#include <boost/any.hpp>
#include <boost/type_traits/is_base_and_derived.hpp>
#include <map>
#include <typeindex>

namespace tpl {
    namespace packets {
        template<class t_value>
        struct metavalue_tag {
            typedef t_value value_type;
            static boost::optional<t_value> no_value;
        };

        template<class t_value>
        boost::optional<t_value> metavalue_tag<t_value>::no_value;

        struct metainfo_data {
            metainfo_data();

            explicit metainfo_data(int flag);

            ~metainfo_data();

            boost::optional<boost::posix_time::ptime> arrival_time_;
			boost::optional<utils::mac_address> source_mac_;
			boost::optional<utils::mac_address> destination_mac_;
            boost::optional<boost::asio::ip::address> source_ip_;
            boost::optional<boost::asio::ip::address> destination_ip_;
            boost::optional<transport_ports> transport_ports_;

            std::map<std::type_index, boost::any> additional_attributes_;


            friend bool operator==(metainfo_data const &lhs, metainfo_data const &rhs) {
                return lhs.arrival_time_ == rhs.arrival_time_
                       && lhs.source_ip_ == rhs.source_ip_
                       && lhs.destination_ip_ == rhs.destination_ip_
                       && lhs.transport_ports_ == rhs.transport_ports_;
            }

            friend bool operator!=(metainfo_data const &lhs, metainfo_data const &rhs) {
                return !(lhs == rhs);
            }
        };

        class TPL_API packet_metainfo {
        public:
            packet_metainfo();

            ~packet_metainfo();

            template<class t_tag>
            boost::optional<typename t_tag::value_type> get() const {
                typedef typename t_tag::value_type target_type;
                static_assert(boost::is_base_and_derived<metavalue_tag<target_type>, t_tag>::value,
                              "t_tag must be derived from metavalue_tag");


                auto iter = this->data_->additional_attributes_.find(std::type_index(typeid(t_tag)));
                if (iter == this->data_->additional_attributes_.end()) {
                    throw std::out_of_range("tag not found");
                }
                return boost::any_cast<target_type>(iter->second);
            }

            template<class t_tag>
            typename t_tag::value_type get_value() const {
                return *get<t_tag>();
            }

            template<class t_tag>
            void set(typename t_tag::value_type const &value) {
                static_assert(boost::is_base_and_derived<metavalue_tag<typename t_tag::value_type>, t_tag>::value,
                              "t_tag must be derived from metavalue_tag");

                this->data_->additional_attributes_[std::type_index(typeid(t_tag))] = value;
            }


            friend bool operator==(packet_metainfo const &lhs, packet_metainfo const &rhs) {
                if (lhs.data_ != nullptr && rhs.data_ != nullptr) {
                    return *lhs.data_ == *rhs.data_;
                }
                return lhs.data_ == rhs.data_;
            }

            friend bool operator!=(packet_metainfo const &lhs, packet_metainfo const &rhs) {
                return !(lhs == rhs);
            }

        private:

            std::shared_ptr<metainfo_data> data_;
        };

#define METAINFO_GENERAL_ATTRIBUTE(TagName, AttributeType)\
        struct TagName : public metavalue_tag<AttributeType>{};

        METAINFO_GENERAL_ATTRIBUTE(gre_call_id_tag, uint16_t)

#define METAINFO_EMBEDDED_ATTRIBUTE(TagName, AttributeType, EmbeddedField)\
        struct TagName : public metavalue_tag<AttributeType>{};\
        template<> inline boost::optional<AttributeType> packet_metainfo::get<TagName>() const { return this->data_->EmbeddedField; }\
        template<> inline void packet_metainfo::set<TagName>(AttributeType const& value) { this->data_->EmbeddedField = value; }


        METAINFO_EMBEDDED_ATTRIBUTE(capture_time_tag, boost::posix_time::ptime, arrival_time_)

		METAINFO_EMBEDDED_ATTRIBUTE(source_mac_tag, utils::mac_address, source_mac_)
		METAINFO_EMBEDDED_ATTRIBUTE(destination_mac_tag, utils::mac_address, destination_mac_)

        METAINFO_EMBEDDED_ATTRIBUTE(source_ip_tag, boost::asio::ip::address, source_ip_)
        METAINFO_EMBEDDED_ATTRIBUTE(destination_ip_tag, boost::asio::ip::address, destination_ip_)

        METAINFO_EMBEDDED_ATTRIBUTE(transport_ports_tag, tpl::packets::transport_ports, transport_ports_)
    }
}

