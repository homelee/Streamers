#include <mon.h>
#include <ml.h>

#include "dbg.h"
#include <net_helper.h>

typedef struct nodeID {
	socketID_handle addr;
	int connID;	// connection associated to this node, -1 if myself
	int refcnt;
	//a quick and dirty static vector for measures TODO: make it dinamic
	MonHandler mhs[10];
	int n_mhs;
} nodeID;


void add_measures(struct nodeID *id)
{
	// Add measures
	int j = 0;
	enum stat_types st[] = {AVG};

	dprintf("adding measures to %s\n",node_addr(id));
	// RX bytes
	id->mhs[j] = monCreateMeasure(BYTE, RXONLY | PACKET | IN_BAND);
	monSetParameter (id->mhs[j], P_PUBLISHING_RATE, 100);
	//Uncomment the following line to publish results
	//monPublishStatisticalType(id->mhs[j], NULL, st , sizeof(st)/sizeof(enum stat_types), NULL);
	monActivateMeasure(id->mhs[j], id->addr, MSG_TYPE_CHUNK);
	j++;
	// TX bytes
	id->mhs[j] = monCreateMeasure(BYTE, TXONLY | PACKET | IN_BAND);
	monSetParameter (id->mhs[j], P_PUBLISHING_RATE, 100);
	//Uncomment the following line to publish results
	//monPublishStatisticalType(id->mhs[j], NULL, st , sizeof(st)/sizeof(enum stat_types), NULL);
	monActivateMeasure(id->mhs[j], id->addr, MSG_TYPE_CHUNK);
	j++;
	/* HopCount */
	id->mhs[j] = monCreateMeasure(HOPCOUNT, TXRXUNI | PACKET | IN_BAND);
	monSetParameter (id->mhs[j], P_PUBLISHING_RATE, 100);
	//Uncomment the following line to publish results
	//monPublishStatisticalType(id->mhs[j], NULL, st , sizeof(st)/sizeof(enum stat_types), NULL);
	monActivateMeasure(id->mhs[j], id->addr, MSG_TYPE_CHUNK);
	j++;
	// for static must not be more then 10 or whatever size is in net_helper-ml.c
	id->n_mhs = j;
}

void delete_measures(struct nodeID *id)
{
	int j;
	dprintf("deleting measures from %s\n",node_addr(id));
	for(j = 0; j < id->n_mhs; j++) {
		monDestroyMeasure(id->mhs[j]);
	}
}