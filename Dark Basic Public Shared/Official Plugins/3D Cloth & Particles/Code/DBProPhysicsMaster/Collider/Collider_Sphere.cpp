#include "stdafx.h"


DECLARE_PLUGIN(Collider_Sphere);

const int Collider_Sphere::classID='SPHR';

Collider_Sphere::Collider_Sphere(int id) : Collider(id),radius(1.0f)
{
}

#ifdef USINGMODULE_P
void Collider_Sphere::onUpdateParticles()
{
	if(emitterList.gotoFirstItem())
	{
		float radiusSq=radius*radius;
		
		emitterInfo.gotoFirstItem();

		ParticleList * alive;
		ParticleList * dead;
		
		do
		{
			ColliderTargetInfo * ei = emitterInfo.getCurItem();
			ei->ticksSinceLastHit++;

			if(!emitterList.getCurItem_WARNING()->isValid())
			{
				objectHasGarbage();				
			}
			else if(!emitterList.getCurItem_WARNING()->isFrozen() && ei->enabled)
			{
				emitterList.getCurItem_WARNING()->getParticleLists(&alive,&dead);

				if(alive->gotoFirstItem())
				{
					Particle * particle;
					if(!ei->invert)
					{
						//Solid sphere
						do
						{
							particle = alive->getCurLink()->data;
							Vector3 diff = particle->pos-m.pos();
							float distSq = diff.MagSqr();
		
							if(distSq<radiusSq)
							{
								ei->ticksSinceLastHit=0;
								ei->numCollisions++;
								if(!(particle->flags&PARTICLEDATA_COLLIDED)) particle->baseFrameCount=particle->frameCount;
								particle->flags|=PARTICLEDATA_COLLIDED;								
								particle->colliderID=objId;

								if(ei->killer) particle->energy=0;
								else
								{
									if(!ei->noResolve)
									{
										float dist=sqrtf(distSq);
										diff/=dist;
										
										Vector3 tmp = diff*(dist-radius);
										particle->pos -= tmp;
										particle->lastPos -= tmp;
										Vector3 vperp= diff * diff.Dot(particle->pos-particle->lastPos);
										Vector3 vpar = ((particle->pos-particle->lastPos)-vperp);
										particle->lastPos=particle->pos-vpar*ei->smoothness+vperp*ei->bounce;
									}
									if(ei->useColor) particle->color=ei->color;
								}
							}
						}while(alive->gotoNextItem());					
					}
					else
					{
						//Spherical cavity
						do
						{
							particle = alive->getCurLink()->data;
							Vector3 diff = m.pos()-particle->pos;
							float distSq = diff.MagSqr();

							if(distSq>radiusSq)
							{
								ei->ticksSinceLastHit=0;
								ei->numCollisions++;
								if(!(particle->flags&PARTICLEDATA_COLLIDED)) particle->baseFrameCount=particle->frameCount;
								particle->flags|=PARTICLEDATA_COLLIDED;								
								particle->colliderID=objId;

								if(ei->killer) particle->energy=0;
								else
								{
									if(!ei->noResolve)
									{
										float dist=sqrtf(distSq);
										diff/=dist;
										
										Vector3 tmp = diff*(radius-dist);
										particle->pos -= tmp;
										particle->lastPos -= tmp;
										Vector3 vperp= diff * diff.Dot(particle->pos-particle->lastPos);
										Vector3 vpar = ((particle->pos-particle->lastPos)-vperp);
										particle->lastPos=particle->pos-vpar*ei->smoothness+vperp*ei->bounce;
									}
									if(ei->useColor) particle->color=ei->color;
								}
							}
						}while(alive->gotoNextItem());					

					}
				}
			}
			emitterInfo.gotoNextItem();
		}
		while(emitterList.gotoNextItem());
	}
}
#endif

#ifdef USINGMODULE_C
void Collider_Sphere::onUpdateCloth()
{
	if(clothList.gotoFirstItem())
	{
		float radiusSq=radius*radius;

		clothInfo.gotoFirstItem();
		
		do
		{
			ColliderTargetInfo * ci = clothInfo.getCurItem();
			//ci->ticksSinceLastHit++;

			if(!clothList.getCurItem_WARNING()->isValid())
			{
				objectHasGarbage();				
			}
			else if(!clothList.getCurItem_WARNING()->isFrozen() && ci->enabled)
			{
				ClothParticle * particle;
				clothList.getCurItem_WARNING()->getParticles(&particle);
				int numParticles = clothList.getCurItem_WARNING()->getNumParticles();

				if(!ci->invert)
				{
					//Solid sphere
					for(int i=0;i<numParticles;i++,particle++)
					{
						Vector3 diff = particle->pos-m.pos();
						float distSq = diff.MagSqr();

						if(distSq<radiusSq)
						{
							//ci->ticksSinceLastHit=0;
							//ci->numCollisions++;
							//particle->flags|=PARTICLEDATA_COLLIDED;
							//particle->colliderID=id;

							if(!ci->noResolve)
							{
								float dist=sqrtf(distSq);
								diff/=dist;
								
								Vector3 tmp = diff*(dist-radius);
								particle->pos -= tmp;
								particle->lastPos -= tmp;
								Vector3 vperp= diff * diff.Dot(particle->pos-particle->lastPos);
								Vector3 vpar = ((particle->pos-particle->lastPos)-vperp);
								particle->lastPos=particle->pos-vpar*ci->smoothness+vperp*ci->bounce;
							}
						}
					}
				}
				else
				{
					//Spherical cavity
					for(int i=0;i<numParticles;i++,particle++)
					{						
						Vector3 diff = m.pos()-particle->pos;
						float distSq = diff.MagSqr();
						if(distSq>radiusSq)
						{
							//ci->ticksSinceLastHit=0;
							//ci->numCollisions++;
							//particle->flags|=PARTICLEDATA_COLLIDED;
							//particle->colliderID=id;

							if(!ci->noResolve)
							{
								float dist=sqrtf(distSq);
								diff/=dist;
								
								Vector3 tmp = diff*(radius-dist);
								particle->pos -= tmp;
								particle->lastPos -= tmp;
								Vector3 vperp= diff * diff.Dot(particle->pos-particle->lastPos);
								Vector3 vpar = ((particle->pos-particle->lastPos)-vperp);
								particle->lastPos=particle->pos-vpar*ci->smoothness+vperp*ci->bounce;
							}
						}
					}
				}
				
			}
			clothInfo.gotoNextItem();
		}
		while(clothList.gotoNextItem());
	}	
}
#endif

#ifdef USINGMODULE_R
void Collider_Sphere::onUpdateRagdoll()
{
	if(ragdollList.gotoFirstItem())
	{
		float radiusSq=radius*radius;

		ragdollInfo.gotoFirstItem();
		
		do
		{
			ColliderTargetInfo * ri = ragdollInfo.getCurItem();
			//ri->ticksSinceLastHit++;

			if(!ragdollList.getCurItem_WARNING()->isValid())
			{
				objectHasGarbage();				
			}
			else if(!ragdollList.getCurItem_WARNING()->isFrozen() && ri->enabled)
			{
				RagdollParticle * particle;
				ragdollList.getCurItem_WARNING()->getParticles(&particle);
				int numParticles = ragdollList.getCurItem_WARNING()->getNumParticles();

				if(!ri->invert)
				{
					//Solid sphere
					for(int i=0;i<numParticles;i++,particle++)
					{
						Vector3 diff = particle->pos-m.pos();
						float distSq = diff.MagSqr();

						if(distSq<radiusSq)
						{
							//ri->ticksSinceLastHit=0;
							//ri->numCollisions++;
							//particle->flags|=PARTICLEDATA_COLLIDED;
							//particle->colliderID=id;

							if(!ri->noResolve)
							{
								float dist=sqrtf(distSq);
								diff/=dist;
								
								Vector3 tmp = diff*(dist-radius);
								particle->pos -= tmp;
								particle->lastPos -= tmp;
								Vector3 vperp= diff * diff.Dot(particle->pos-particle->lastPos);
								Vector3 vpar = ((particle->pos-particle->lastPos)-vperp);
								particle->lastPos=particle->pos-vpar*ri->smoothness+vperp*ri->bounce;
							}
						}
					}
				}
				else
				{
					//Spherical cavity
					for(int i=0;i<numParticles;i++,particle++)
					{						
						Vector3 diff = m.pos()-particle->pos;
						float distSq = diff.MagSqr();
						if(distSq>radiusSq)
						{
							//ri->ticksSinceLastHit=0;
							//ri->numCollisions++;
							//particle->flags|=PARTICLEDATA_COLLIDED;
							//particle->colliderID=id;

							if(!ri->noResolve)
							{
								float dist=sqrtf(distSq);
								diff/=dist;
								
								Vector3 tmp = diff*(radius-dist);
								particle->pos -= tmp;
								particle->lastPos -= tmp;
								Vector3 vperp= diff * diff.Dot(particle->pos-particle->lastPos);
								Vector3 vpar = ((particle->pos-particle->lastPos)-vperp);
								particle->lastPos=particle->pos-vpar*ri->smoothness+vperp*ri->bounce;
							}
						}
					}
				}
				
			}
			ragdollInfo.gotoNextItem();
		}
		while(ragdollList.gotoNextItem());
	}	
}
#endif

ePhysError Collider_Sphere::setRadius(float rad)
{
	if(rad<0.0f) return PERR_BAD_PARAMETER;
	radius=rad;
	return PERR_AOK;
}



//-----------------------------------------------------------------------------
//--- Commands ----------------------------------------------------------------
//-----------------------------------------------------------------------------



DLLEXPORT void makeCollisionSphere(int id)
{
	ON_FAIL_DLL_SECURITY_RETURN(void)

	ALERT_ON_PERR(physics->addCollider(id,COLLIDER_PLUGIN(Collider_Sphere)),PCMD_MAKE_COLLISION_SPHERE);
}


DLLEXPORT void setCollisionSphere(int id, float rad)
{
	ColliderPtr tmp;
	RETURN_ON_PERR(physics->getCollider(id,tmp),PCMD_SET_COLLISION_SPHERE);
	// mike
	//RETURN_ON_PERR(tmp->checkType(Collider::classID,Collider_Sphere::classID),PCMD_SET_COLLISION_SPHERE);

	Collider_Sphere * sph = reinterpret_cast<Collider_Sphere *>(&(*tmp));
	RETURN_ON_PERR(sph->setRadius(rad),PCMD_SET_COLLISION_SPHERE);
}

