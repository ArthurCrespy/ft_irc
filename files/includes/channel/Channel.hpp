/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdegluai <jdegluai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 09:46:27 by acrespy           #+#    #+#             */
/*   Updated: 2024/06/10 14:48:46 by jdegluai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

class Client;

class Channel
{
	private:
		std::string			_channel_name;
		std::string			_channel_topic;
		bool				_channel_topic_set;
		bool				_channel_topic_restrict;
		std::string			_channel_password;
		bool				_channel_password_restrict;
		int					_channel_limit;
		bool				_channel_invite_only;
		std::vector<char>			_modes;
		std::vector<std::string>	_inviteList;

		t_members			_channel_members;
		t_members			_channel_admins;

		Client				*_owner;

	public:
		Channel(void);
		Channel(std::string const &name, Client *op);
		Channel(Channel const &src);
		~Channel(void);

		Channel &operator=(Channel const &rhs);

		void setTopic(std::string const &topic);
		void setTopicSet(bool set);
		void setTopicRestriction(bool restrict);
		void setPassword(std::string const &password);
		void setPasswordRestriction(bool restrict);
		void setLimit(int limit);
		void setInviteOnly(bool io);
		void setOwner(Client *owner);

		bool	isInvited(Client *user) const;
		std::string	getName(void) const;
		std::string	getTopic(void) const;
		bool		getTopicSet(void) const;
		bool		getTopicRestriction(void) const;
		std::string	getPassword(void) const;
		bool		getPasswordRestriction(void) const;
		int			getLimit(void) const;
		bool		getInviteOnly(void) const;
		bool		hasMode(char mode) const;
		// int getMembersNb
		t_members	getMembers(void) const;
		t_members	getAdmins(void) const;

		void addMember(Client *member);
		void removeMember(Client *member);
		void removeMember(std::string const &member);

		void addAdmin(Client *op);
		void removeAdmin(Client *op);
		void removeAdmin(std::string const &op);

		void broadcast(std::string const &msg);
};

#endif
