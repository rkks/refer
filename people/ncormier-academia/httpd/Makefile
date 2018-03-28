# Makefile


main:
	@echo "ModAnswering ..."
	@( cd Modules/unix/ModAnswering && gmake)
	@echo ""
	@echo "ModListDir ..."
	@( cd Modules/unix/ModListDir  && gmake)
	@echo ""
	@echo "ModListenerSSL ..."
	@( cd Modules/unix/ModListenerSSL  && gmake)
	@echo ""
	@echo "ModNetworkSSL ..."
	@( cd Modules/unix/ModNetworkSSL  && gmake)
	@echo ""
	@echo "ModFileReg ..."
	@( cd Modules/unix/ModFileReg  && gmake)
	@echo ""
	@echo "ModListener ..."
	@( cd Modules/unix/ModListener  && gmake)
	@echo ""
	@echo "ModNetwork ..."
	@( cd Modules/unix/ModNetwork  && gmake)
	@echo ""
	@echo "ModProtocol ..."
	@( cd Modules/unix/ModProtocol  && gmake)
	@echo ""
	@echo "tinyxml ..."
	@( cd lib/tinyxml/ && gmake)
	@echo ""
	@echo "Zia ..."
	@gmake -f Makefile.unix
	@rm -f config/config.xml
	@cp config/config.xml.unix config/config.xml

clean:
	@echo "Cleaning ModAnswering ..."
	@( cd Modules/unix/ModAnswering && gmake clean)
	@echo ""
	@echo "Cleaning ModListDir ..."
	@( cd Modules/unix/ModListDir  && gmake clean)
	@echo ""
	@echo "Cleaning ModListenerSSL ..."
	@( cd Modules/unix/ModListenerSSL  && gmake clean)
	@echo ""
	@echo "Cleaning ModNetworkSSL ..."
	@( cd Modules/unix/ModNetworkSSL  && gmake clean)
	@echo ""
	@echo "Cleaning ModFileReg ..."
	@( cd Modules/unix/ModFileReg  && gmake clean)
	@echo ""
	@echo "Cleaning ModListener ..."
	@( cd Modules/unix/ModListener  && gmake clean)
	@echo ""
	@echo "Cleaning ModNetwork ..."
	@( cd Modules/unix/ModNetwork  && gmake clean)
	@echo ""
	@echo "Cleaning ModProtocol ..."
	@( cd Modules/unix/ModProtocol  && gmake clean)
	@echo ""
	@echo "Cleaning tinyxml ..."
	@( cd lib/tinyxml/ && gmake clean)
	@echo ""
	@echo "Cleaning Zia ..."
	@gmake -f Makefile.unix clean
