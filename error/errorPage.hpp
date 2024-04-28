/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errorPage.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jama <sel-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 20:59:00 by sel-jama          #+#    #+#             */
/*   Updated: 2024/04/28 19:05:53 by sel-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Request/Global.hpp"

class Request;
class errorPage{
    private:
        std::string msg;
        int code;
        std::string img;
        std::string color;
        std::string boxRgba;

        // std::string page;
        
    public:
        std::map<int , std::string> errorMsgs;
        errorPage();
        errorPage(std::string msg, int code);
        ~errorPage();

        void HtmlErrorPage();
        void setProprties();
        void setErrorMsgs();

        static std::string serveErrorPage(Request &);
};