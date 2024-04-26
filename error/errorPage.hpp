/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errorPage.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jama <sel-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 20:59:00 by sel-jama          #+#    #+#             */
/*   Updated: 2024/04/26 07:42:52 by sel-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Request/Global.hpp"

class errorPage{
    private:
        std::string msg;
        int code;
        std::string img;
        std::string color;
        std::string boxRgba;
        std::map<int , std::string> errorMsgs;

        // std::string page;
        
    public:
        errorPage(std::string msg, int code);
        ~errorPage();

        void HtmlErrorPage();
        void setProprties();
        void setErrorMsgs();
};