#ifndef _SM_PUBLIC_H_
#define _SM_PUBLIC_H_

enum{
	vendor_fm=1,

	max_verdor,
};

enum fm_type_e {
	dev_type_pci_1_2 = 1,
	dev_type_pci_2_0,
	dev_type_pcie_1_0,
	dev_type_pcie_2_0,

	max_card_type,
};

/* TODO: OS_RUNNING_DIR instead of '/secgate' */
#define SM_CONF_FILE	"/secgate/app/openssl/plugins/smcard.conf"

#define FM_API_LIB	"/secgate/app/openssl/plugins/fmcard/libfmapiv100.so"

#define SMCARD_VENDOR	"vendor"
#define SMCARD_TYPE	"type"

/* return: 0 - not exist, others - exist */
static inline int does_smcard_exist(void)
{
	char cmd[128];

	sprintf(cmd, "/bin/grep \"%s = 0\" %s >/dev/null", SMCARD_VENDOR, SM_CONF_FILE);
	return system(cmd);
}

static inline int get_smcard_info(int *vendor, int *dev_type)
{
	FILE *fp = NULL;
	char buf[128]={0}, cmd[128];

	/* read card vendor from hw config file */
	sprintf(cmd, "/bin/grep \"%s\" %s | awk {'print $3'}", SMCARD_VENDOR, SM_CONF_FILE);
	if ((fp = popen(cmd, "r")) == NULL) {
		return -1;
	}
	memset(buf, 0, sizeof(buf));
	fgets(buf, sizeof(buf), fp);
	pclose(fp);

	*vendor = atoi(buf);
	if(*vendor == 0) {
		return -1;
	}

	/* read card subtype from hw config file */
	sprintf(cmd, "/bin/grep \"%s\" %s | awk {'print $3'}", SMCARD_TYPE, SM_CONF_FILE);
	if ((fp = popen(cmd, "r")) == NULL) {
		return -1;
	}
	memset(buf, 0, sizeof(buf));
	fgets(buf, sizeof(buf), fp);
	pclose(fp);

	*dev_type = atoi(buf);
	return 0;
}

static inline void save_smcard_info(int vendor, int type)
{
	char cmd[128];

	snprintf(cmd, sizeof(cmd), "echo \"%s = %d\" > \"%s\"", SMCARD_VENDOR, vendor, SM_CONF_FILE);
	system(cmd);
	snprintf(cmd, sizeof(cmd), "echo \"%s = %d\" >> \"%s\"", SMCARD_TYPE, type, SM_CONF_FILE);
	system(cmd);
}

#endif
