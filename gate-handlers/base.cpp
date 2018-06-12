#include<Poco/JSON/Array.h>
#include<Poco/JSON/Parser.h>

#include"base.h"

using Poco::JSON::Object;
using Poco::JSON::Array;
using Poco::JSON::Parser;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;

const Object::Ptr BaseRequestHandler::ReadRequestBody(HTTPServerRequest &request){
	return nullptr;
}

void BaseRequestHandler::BadRequest(HTTPServerResponse &response){
    response.setStatusAndReason(HTTPServerResponse::HTTP_BAD_REQUEST,HTTPServerResponse::HTTP_REASON_BAD_REQUEST);
}

void BaseRequestHandler::SendResponse(HTTPServerResponse &response,const Object::Ptr data){
    Object::Ptr dataCopy(new Object(*data));
    
}