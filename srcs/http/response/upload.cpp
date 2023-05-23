#include "Response.hpp"
extern bool running;

std::string	Response::extractMPFD_Header()
{
	/*get the header of multi part form data*/
	std::vector<unsigned char>::iterator it = (*_content).begin();

	while (it != (*_content).end() && _controler.find("\r\n\r\n") == std::string::npos)
	{
		_controler += *it;
		it = (*_content).erase(it);
	}
	return (_controler);
}

void	Response::uploadFailed()
{
	std::vector<std::string>::iterator	it = _files.begin();
	std::string	path;

	_code = 400;
	//_uploadStatu = STOP;
	_uploadStatu = EMPTY;
	while (it != _files.end())
	{
		path = _uploadPath + *it;
		if (isFile(path))
		{
			if (remove(path.c_str()) != 0)
				_code = 403;
		}
		it++;
	}
}

void	Response::uploadSuccess()
{
	std::string	newRequestContent = "";

	std::vector<std::string>::iterator	it = _files.begin();
	std::vector<std::string>::iterator	yt = _headerName.begin();

	while (it != _files.end() && yt != _headerName.end())
	{
		newRequestContent += "&" + *yt + "=" + _uploadPath + *it;
		yt++;
		it++;
	}
	if (newRequestContent[0] == '&')
		newRequestContent.erase(0, 1);
	_request.setRequestContent(newRequestContent);
	//_uploadStatu = STOP;
	_uploadStatu = EMPTY;
}

void	Response::upload()
{
	/*if _uploadStatu == START set the boundary, uploadPath and controler to not redefine them next time*/
	if (_uploadStatu == EMPTY)
	{
		std::string	Content_type = _request.getElInHeader("Content-Type");
		if (Content_type.find("multipart/form-data") != std::string::npos)
		{
			size_t	boundaryPos = Content_type.find("boundary=");
			if (boundaryPos != std::string::npos)
			{
				_boundary = Content_type.substr(boundaryPos + 9);
				if (_boundary[_boundary.size() - 1] == '\r')
					_boundary.erase(_boundary.size() - 1, 1);
			}
			else
				return ;
			_uploadPath = _location.getUploadPath();
			_controler = "";
		}
		else
			return ;
	}

	/*================================================================================================*/

	/*know we need to open the file on which we gonna write the _content */
	std::ofstream	outfile;

	    /*if the _uploadFileName is already set we just open it*/
	if (_uploadFileName != "")
	{
		if (isFile(_uploadPath + _uploadFileName) == false)
		{
			uploadFailed();
			return ;
		}
		outfile.open(_uploadPath + _uploadFileName, std::ios_base::app);
		if (outfile.is_open() == false)
		{
			uploadFailed();
			return ;
		}
	}
	    /*===================================================*/
		/*otherwise it means that we haven't read the form header yet */
	else
	{
		std::string header = extractMPFD_Header(); // take the header
		if (header.find("\r\n\r\n") == std::string::npos) // if we dont have "\r\n\r\n" 
		{												 //it means that we have not read all the content of the header 
			_uploadStatu = READ;						// so we put the _uploadStatu to WAITING 
			return ;								   // to tell the server that we are still waiting for data
		}
		std::string			varName = "";
		size_t				varNameStartPos = header.find("name=\"") + 6;
		size_t				varNameEndPos = header.find("\"", varNameStartPos);
		if (varNameStartPos != std::string::npos && varNameEndPos != std::string::npos
			&& varNameEndPos > varNameStartPos)
			varName = header.substr(varNameStartPos, varNameEndPos - varNameStartPos);
		else
		{
			uploadFailed();
			return ;
		}
		_headerName.push_back(varName);

		/*get the name of the file in the header*/
		size_t				filenameStartPos = header.find("filename=\"") + 10;
		size_t				filenameEndPos = header.find("\"", filenameStartPos);
		if (filenameStartPos != std::string::npos && filenameEndPos != std::string::npos
			&& filenameEndPos > filenameStartPos)
			_uploadFileName = header.substr(filenameStartPos, filenameEndPos - filenameStartPos);
		else /*if there is not "filename=" in the header it means that an other type of data was send with mulit part form data so error*/
		{
			uploadFailed();
			return ;
		}
		/*===================================*/
		outfile.open(_uploadPath + _uploadFileName);
		if (outfile.is_open() == false)
		{
			uploadFailed();
			return ;
		}
		_files.push_back(_uploadFileName);
		_controler = "";
	}
	    /*=============================================================*/
	/*===================================================================*/

	/*setup the _controler which will take boundary.size() ahead of _content to check if we reach the end of a form*/
	std::vector<unsigned char>::iterator	it = (*_content).begin();
	if (_controler.size() < (_boundary.size() + 4))
	{
		while (it != (*_content).end() && _controler.size() < (_boundary.size() + 4))
		{
			_controler.push_back(*it);
			it++;
		}
	}
	/*===========================================================================================================*/
	std::vector<unsigned char>::iterator	yt = (*_content).begin();
	/*in this loop while checking if we arrive at the end of _content or at the end of the all form 
	or of a form we will increment size which will allow us to have the complete size of the body, 
	we will also update the control while removing its first element then adding the character
	 *it in the file in question*/
	while (running && (yt != (*_content).end() && (_controler.find(_boundary + "\r\n") == std::string::npos 
		&& _controler.find(_boundary + "--") == std::string::npos)))
	{
		if (it != (*_content).end())
		{
			_controler.push_back(*it);//
			_controler.erase(0, 1);//
		}
		outfile << *yt;
		yt = (*_content).erase(yt);
	}
	outfile.close();
	if (running == false)
		return ;
	/*_boundary + -- means we have completed all forms so we can stop the upload process*/
	if (_controler.find(_boundary + "--") != std::string::npos)
	{
		uploadSuccess();
		return ;
	}
	/*_boundary + \r\n means we have completed an form so we reset _uploadFilname and _cotroler*/
	else if (_controler.find(_boundary + "\r\n") != std::string::npos)
	{
		_uploadFileName = "";
		_controler = "";
	}
	/*if we reach the end of _content we set the status to waiting */
	if (it == (*_content).end())
	{
		_uploadStatu = READ;
		return ;
	}
	/*it means that we can continue the download process so we call upload() recursively*/
	upload();
}
