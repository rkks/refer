#ifndef   __MOZIA_ITRANSFORM_
# define  __MOZIA_ITRANSFORM_

namespace	mozia
{
  namespace Transform
    {
      class ITransform
	{
	public:
	  // VFunc
	  virtual ~ITransform(void)
	    {
	    }
	  virtual	char	*Encode(char *data) = 0;
	  virtual	char	*Decode(char *data) = 0;
	};
    }
}

#endif /* __MOZIA_ITRANSFORM_ */
