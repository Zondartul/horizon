

















#ifndef SVTTENG_H_
#define SVTTENG_H_

#include FT_INTERNAL_SERVICE_H
#include FT_MODULE_H


FT_BEGIN_HEADER


  

#define FT_SERVICE_ID_TRUETYPE_ENGINE  "truetype-engine"

  

  FT_DEFINE_SERVICE( TrueTypeEngine )
  {
    FT_TrueTypeEngineType  engine_type;
  };

  


FT_END_HEADER


#endif 



