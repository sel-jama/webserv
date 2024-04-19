/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errorPage.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jama <sel-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 20:59:00 by sel-jama          #+#    #+#             */
/*   Updated: 2024/04/18 21:08:30 by sel-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request/Global.hpp"

class errorPage{
    private:
        std::string msg;
        int code;

        std::string page;
        
    public:
        errorPage(std::string msg, int code);
        ~errorPage();

        void HtmlErrorPage();
};