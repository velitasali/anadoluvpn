/**
 * @file configuration_manager.h
 * 
 * @brief Interface of configuration_manager_t.
 *  
 */

/*
 * Copyright (C) 2005 Jan Hutter, Martin Willi
 * Hochschule fuer Technik Rapperswil
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.  See <http://www.fsf.org/copyleft/gpl.txt>.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 */

#ifndef CONFIGURATION_MANAGER_H_
#define CONFIGURATION_MANAGER_H_

#include <types.h>
#include <config/init_config.h>
#include <config/sa_config.h>
#include <transforms/rsa/rsa_private_key.h>
#include <transforms/rsa/rsa_public_key.h>


typedef struct configuration_manager_t configuration_manager_t;

/**
 * @brief Manages all configuration aspects of the daemon.
 * 
 * @b Constructors:
 * 	- configuration_manager_create()
 * 
 * @todo Build a (file) backend for the configuration manager.
 * 
 * @ingroup config
 */
struct configuration_manager_t { 

	/**
	 * @brief Returns the configuration information needed for IKE_SA_INIT exchange 
	 * for a specific configuration name.
	 * 
	 * The returned init_config_t object MUST NOT be destroyed cause it's managed by 
	 * this configuration_manager_t object.
	 * 
	 * @param this				calling object
	 * @param name				name of the configuration
	 * @param[out] init_config	the init_config_t object is stored at this location
	 * 
	 * @return		
	 * 							- NOT_FOUND
	 * 							- SUCCESS
	 */
	status_t (*get_init_config_for_name) (configuration_manager_t *this, char *name, init_config_t **init_config);

	/**
	 * @brief Returns the configuration information needed for IKE_SA_INIT exchange 
	 * for specific host informations.
	 * 
	 * The returned init_config_t object MUST NOT be destroyed cause it's managed by 
	 * this configuration_manager_t object.
	 * 
	 * @param this				calling object
	 * @param my_host			my host informations
	 * @param other_host		other host informations
	 * @param[out] init_config	the init_config_t object is stored at this location
	 * 
	 * @return		
	 * 							- NOT_FOUND
	 * 							- SUCCESS
	 */	
	status_t (*get_init_config_for_host) (configuration_manager_t *this, host_t *my_host, host_t *other_host,init_config_t **init_config);
	
	/**
	 * @brief Returns the configuration information needed after IKE_SA_INIT exchange 
	 * for a specific configuration name.
	 * 
	 * The returned sa_config_t object MUST NOT be destroyed cause it's managed by 
	 * this configuration_manager_t object.
	 * 
	 * @param this				calling object
	 * @param name				name of the configuration
	 * @param[out] sa_config	the sa_config_t object is stored at this location
	 * 
	 * @return		
	 * 				- NOT_FOUND
	 * 				- SUCCESS
	 */
	status_t (*get_sa_config_for_name) (configuration_manager_t *this, char *name, sa_config_t **sa_config);
	
	/**
	 * @brief Returns the configuration information needed after IKE_SA_INIT exchange 
	 * for specific init_config_t and ID data.
	 * 
	 * The returned sa_config_t object MUST NOT be destroyed cause it's managed by 
	 * this configuration_manager_t object.
	 * 
	 * @param this				calling object
	 * @param init_config		init_config_t object
	 * @param other_id			identification of other one
	 * @param my_id				my identification (can be NULL)
	 * @param[out] sa_config	the sa_config_t object is stored at this location
	 * 
	 * @return		
	 * 							- NOT_FOUND
	 * 							- SUCCESS
	 */	
	status_t (*get_sa_config_for_init_config_and_id) (configuration_manager_t *this, init_config_t *init_config, identification_t *other_id, identification_t *my_id,sa_config_t **sa_config);

	/**
	 * @brief Returns the retransmit timeout.
	 * 
	 * The timeout values are managed by the configuration manager.
	 * 
	 * @param this				calling object
	 * @param retransmit_count	number of times a message was retransmitted so far
	 * @param[out] timeout		the new retransmit timeout in milliseconds
	 * 
	 * @return		
	 * 							- FAILED, if the message should not be retransmitted
	 * 							- SUCCESS
	 */
	status_t (*get_retransmit_timeout) (configuration_manager_t *this, u_int32_t retransmit_count, u_int32_t *timeout);
	
	/**
	 * @brief Returns the timeout for an half open IKE_SA in ms.
	 * 
	 * Half open means that the IKE_SA is still in one of the following states:
	 *  - INITIATOR_INIT
	 *  - RESPONDER_INIT
	 *  - IKE_SA_INIT_REQUESTED
	 *  - IKE_SA_INIT_RESPONDED
	 *  - IKE_AUTH_REQUESTED
	 * 
	 * @param this				calling object
	 * @return					timeout in milliseconds (ms)
	 */	
	u_int32_t (*get_half_open_ike_sa_timeout) (configuration_manager_t *this);
	
	/**
	 * @brief Returns the preshared secret of a specific ID.
	 * 
	 * The returned preshared secret MUST NOT be destroyed cause it's managed by 
	 * this configuration_manager_t object.
	 * 
	 * @param this					calling object
	 * @param identification		identification_t object identifiying the ID.
	 * @param[out] preshared_secret	the preshared secret will be written there.
	 * 
	 * @return		
	 * 								- NOT_FOUND	if no preshared secrets for specific ID could be found
	 * 								- SUCCESS
	 */	
	status_t (*get_shared_secret) (configuration_manager_t *this, identification_t *identification, chunk_t *preshared_secret);
	
	/**
	 * @brief Returns the RSA public key of a specific ID.
	 * 
	 * The returned rsa_public_key_t object MUST NOT be destroyed cause it's managed by 
	 * this configuration_manager_t object.
	 * 
	 * @param this					calling object
	 * @param identification		identification_t object identifiying the ID.
	 * @param[out] public_key		the public key will be written there
	 * 
	 * @return		
	 * 								- NOT_FOUND	if no key is configured for specific id
	 * 								- SUCCESS
	 */	
	status_t (*get_rsa_public_key) (configuration_manager_t *this, identification_t *identification, rsa_public_key_t **public_key);
	
	/**
	 * @brief Returns the RSA private key of a specific ID.
	 * 
	 * The returned rsa_private_key_t object MUST NOT be destroyed cause it's managed by 
	 * this configuration_manager_t object.
	 * 
	 * @param this					calling object
	 * @param identification		identification_t object identifiying the ID.
	 * @param[out] private_key		the private key will be written there
	 * 
	 * @return		
	 * 								- NOT_FOUND	if no key is configured for specific id
	 * 								- SUCCESS
	 */	
	status_t (*get_rsa_private_key) (configuration_manager_t *this, identification_t *identification, rsa_private_key_t **private_key);

	/**
	 * @brief Destroys a configuration_manager_t object.
	 * 
	 * @param this 					calling object
	 */
	void (*destroy) (configuration_manager_t *this);
};

/**
 * @brief Creates the mighty configuration manager.
 * 
 * @param first_retransmit_timeout 	first retransmit timeout in milliseconds
 * @param max_retransmit_count		max number of tries to retransmitted a requests (0 for infinite)
 * @param half_open_ike_sa_timeout  timeout after that a half open IKE_SA gets deleted
 * @return 							configuration_manager_t object
 * 
 * @ingroup config
 */
configuration_manager_t *configuration_manager_create(u_int32_t first_retransmit_timeout,u_int32_t max_retransmit_count, u_int32_t half_open_ike_sa_timeout);

#endif /*CONFIGURATION_MANAGER_H_*/
