source ~/.bashrc
source ~/.bash_logout
case "$OS" in
    IRIX)
	stty sane dec
	stty erase
	;;
# SunOS
#     # stty erase
      # ;;
    *)
	stty erase
	;;
esac
