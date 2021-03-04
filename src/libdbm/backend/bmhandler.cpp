#include "bmhandler.h"

const QString BMHandler::errorString(BMHandler::ErrorType et)
{
    QString strErr;

    switch (et) {
    case NoError:
        strErr = "";
        break;
    case USBFormatError:
        strErr = BMHandler::tr("Disk Format Error: Please reformat it");
        break;
    case USBSizeError:
        strErr = BMHandler::tr("Insufficient disk Space: Ensure that the disk has enough free space");
        break;
    case USBMountFailed:
        strErr = BMHandler::tr("Disk Mount Error: Insert the disk again or reboot to retry");
        break;
    case ExtractImgeFailed:
        strErr = BMHandler::tr("Image Decompression Error: Verify md5 checksum of the image to ensure its integrity");
        break;
    default:
        strErr = BMHandler::tr("Internal Error");
        break;
    }

    return strErr;
}
