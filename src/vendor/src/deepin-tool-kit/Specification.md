# Deepin Application Specification   {#Specification}

Every application should keep the rule in this document.

## 1. Application Information

Application should set the property Organization-name and Application-name.

Organization/Application name can contains alphabet, number and other visible ASCII code, BUT space MUST NOT appear in the name. And we do not approve
of no-ASCII code character in the Organization/Application name

Application can stay Organization-name empty, but it should always set an Application-name.

## 2. Standard Path

The log, configure and runtime cache of Application should store in specific path.

**If Organization-name is empty, "{Organization-name}/" would not appear in path.**

### 2.1 User Application Standard Path

As an application run for user session, the Standard path should be:

````bash
XGD_LOG_HOME_DEEPIN:
    Where deepin-user-specific log should be written.
    XGD_USER_HOME/.log

DAPP_CONFIG_HOME:
    Where application-specific configurations should be written.
    XDG_CONFIG_HOME/{Organization-name}/{Application-name}

DAPP_LOG_HOME:
    Where application-specific log should be written.
    XGD_LOG_HOME_DEEPIN/{Organization-name}/{Application-name}

DAPP_CACHE_HOME:
    Where application-specific cache files should be written.
    XGD_CACHE_HOME/{Organization-name}/{Application-name}

DAPP_DATA_HOME:
    Where application-specific data files should be written.
    $XDG_DATA_HOME/{Organization-name}/{Application-name}

````

Simple example:

The dde-dock is offcial application of deepin and the standard path will be:

````bash
DAPP_CONFIG_HOME:   $HOME/.config/deepin/dde-dock
DAPP_LOG_HOME:      $HOME/.log/deepin/dde-dock
DAPP_CACHE_HOME:    $HOME/.cache/deepin/dde-dock
DAPP_DATA_HOME:     $HOME/.local/share/deepin/dde-dock
````

### 2.2 System Application Standard Path

Application run as system daemon, or with user with no home directory should place it's file in this standard path:

````bash
DAPP_CONFIG_SYS:
    /etc/{Organization-name}/{Application-name}

DAPP_LOG_SYS:
    /var/log/{Organization-name}/{Application-name}

DAPP_DATA_SYS:
    /usr/share/{Organization-name}/{Application-name}

DAPP_CACHE_SYS:
    /var/cache/{Organization-name}/{Application-name}
````


Refs:

[XDG Base Directory Specification](https://specifications.freedesktop.org/basedir-spec/basedir-spec-0.8.html)

[XDG Base Directory support](https://wiki.archlinux.org/index.php/XDG_Base_Directory_support)
